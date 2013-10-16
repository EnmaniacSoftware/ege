#include <EGEApplication.h>
#include <EGEMath.h>
#include <EGEDebug.h>
#include <EGEResources.h>
#include <EGERenderComponent.h>
#include <EGERenderer.h>
#include "LightningEffectStrips.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(LightningEffectStrips)
EGE_DEFINE_DELETE_OPERATORS(LightningEffectStrips)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
LightningEffectStrips::LightningEffectStrips(Application* app) : Object(app),
                                                                 m_maximumOffset(60.0f),
                                                                 m_offshotAngle(EGEMath::PI * 0.25f),
                                                                 m_offshotAngleVariance(EGEMath::PI * 0.125f),
                                                                 m_state(STATE_NONE),
                                                                 m_width(1.0f),
                                                                 m_renderPriority(EGEGraphics::RP_MAIN),
                                                                 m_randomizationVariance(0.0f),
                                                                 m_randomizationInterval(-1.0f)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
LightningEffectStrips::~LightningEffectStrips()
{
  clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool LightningEffectStrips::isValid() const
{
  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Renders object. */
void LightningEffectStrips::render(IRenderer* renderer)
{
  if (STATE_BUSY == m_state)
  {
    // go thru all beams
    for (BeamList::const_iterator it = m_beams.begin(); it != m_beams.end(); ++it)
    {
      const Beam& beam = *it;

      renderer->addForRendering(beam.renderData);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates effect. */
void LightningEffectStrips::update(const Time& time)
{
  if (STATE_BUSY == m_state)
  {
    m_fadeTime += time;
    if (m_fadeTime.seconds() > 1.0f)
    {
      m_state = STATE_IDLE;
    }

    // check if randomization is to be updated
    bool randomize = false;
    if (0 <= m_randomizationInterval.microseconds())
    {
      m_randomizationTime -= time;
      if (0 >= m_randomizationTime.microseconds())
      {
        randomize = true;
        m_randomizationTime = m_randomizationInterval;
      }
    }

    // go thru all beams
    for (BeamList::iterator it = m_beams.begin(); it != m_beams.end(); ++it)
    {
      Beam& beam = *it;

      float32* data = reinterpret_cast<float32*>(beam.renderData->vertexBuffer()->lock(0, 2 + beam.segments.size() * 2));

      // go thru all segments
      u32 i = 0;
      for (SegmentList::iterator itSegment = beam.segments.begin(); itSegment != beam.segments.end(); ++itSegment, ++i)
      {
        Segment& segment = *itSegment;

        bool isFirst = (i == 0);
        bool isLast  = (i == beam.segments.size() - 1);

        if (isFirst)
        {
          *data++;
          *data++;
          *data++;
          *data++;
          *data++;
          *data++;
          *data++;
          *data++ = segment.intensity * (1.0f - m_fadeTime.seconds());

          *data++;
          *data++;
          *data++;
          *data++;
          *data++;
          *data++;
          *data++;
          *data++ = segment.intensity * (1.0f - m_fadeTime.seconds());
        }

        float32 oldOffset = segment.randomization;
        if (randomize)
        {
          segment.randomization = isLast ? 0.0f : Math::Random()(-m_randomizationVariance, m_randomizationVariance);
        }

        // NOTE: *data++ = *data... is apparently not entirely defined operation if one uses pre or post incrementation of a variable which is used 
        //       somehwere else in the expression too. Thus, decoupled into 2 statements
        *data = *data - (oldOffset - segment.randomization) * segment.randomizationNormal.x;
        ++data;
        *data = *data - (oldOffset - segment.randomization) * segment.randomizationNormal.y;
        ++data;
        *data++;
        *data++;
        *data++;
        *data++;
        *data++;
        *data++ = segment.intensity * (1.0f - m_fadeTime.seconds());

        *data = *data - (oldOffset - segment.randomization) * segment.randomizationNormal.x;
        ++data;
        *data = *data - (oldOffset - segment.randomization) * segment.randomizationNormal.y;
        ++data;
        *data++;
        *data++;
        *data++;
        *data++;
        *data++;
        *data++ = segment.intensity * (1.0f - m_fadeTime.seconds());
      }

      beam.renderData->vertexBuffer()->unlock(data - 1);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Generates segments. */
void LightningEffectStrips::create(const List<Vector2f>& points, s32 steps, bool allowOffshots)
{
  // check if at least 2 points present
  if (2 > points.size())
  {
    // done
    return;
  }

  m_beams.clear();
  m_beams.push_back(Beam());

  // create initial segments
  List<Vector2f>::const_iterator it = points.begin();
  Segment segment;
  segment.end = *it++;
  for (; it != points.end(); ++it)
  {
    segment.start               = segment.end;
    segment.end                 = *it;
    segment.normal              = (segment.end - segment.start).perpendicular().normalized();
    segment.intensity           = 1.0f;
    segment.randomization       = 0.0f;
    segment.randomizationNormal = Vector2f::ZERO;
  
    m_beams.back().segments.push_back(segment);
  }

  s32 a = 0;
  float32 offset = m_maximumOffset;
  for (s32 step = 0; step < steps; ++step)
  {
    // go thru all beams
    for (BeamList::iterator it = m_beams.begin(); it != m_beams.end(); ++it)
    {
      Beam& beam = *it;

      for (SegmentList::iterator itSegment = beam.segments.begin(); itSegment != beam.segments.end(); ++itSegment)
      {
        Segment& oldSegment = *itSegment;

        // determine segment vector
        Vector2f segmentVector = oldSegment.end - oldSegment.start;

        // get point in the middle of segment
        Vector2f midPoint(oldSegment.start.x + segmentVector.x * 0.5f, oldSegment.start.y + segmentVector.y * 0.5f);

        // calulate normalized vector perpendicular to current segment
        Vector2f offsetVector = segment.normal;
        //if (m_random() & 0x1)
        if (a == 0)
        {
          offsetVector *= -1.0f;
        }
        a++;

        // move mid point along the perpendicular vectors
        midPoint += offsetVector * offset;

        // create new segment for first subsegment
        segment.start               = oldSegment.start;
        segment.end                 = midPoint;
        segment.normal              = (segment.end - segment.start).perpendicular().normalized();
        segment.intensity           = oldSegment.intensity;
        segment.randomization       = 0.0f;
        segment.randomizationNormal = Vector2f::ZERO;

        // update current segment to second subsegment
        oldSegment.start = midPoint;
        oldSegment.end  = oldSegment.end;
        oldSegment.normal = (oldSegment.end - midPoint).perpendicular().normalized();

        beam.segments.insert(itSegment, segment);

        // check if offshot should be generated
        if (allowOffshots)
        {
          if ((Math::Random()() % 100) < oldSegment.intensity * 50)
          {
            Beam offshotBeam;

            segment.start = midPoint;

            Vector2f direction = oldSegment.end - midPoint;
            float32 dirLength = direction.length();
            direction.normalize();

            Angle angle = Angle::FromDegrees(Math::Random()(5, 10));

            float32 cos = Math::Cos(angle.radians());
            float32 sin = Math::Sin(angle.radians());

            segment.end.x = direction.x * cos - direction.y * sin;
            segment.end.y = direction.x * sin + direction.y * cos;

            segment.end     = segment.end * dirLength * 0.7f + midPoint;
            segment.normal  = (segment.end - segment.start).perpendicular().normalized();
            segment.intensity = oldSegment.intensity * 0.5f;
            offshotBeam.segments.push_back(segment);

            m_beams.push_front(offshotBeam);
          }
        }
      }
    }

    // update offset magnitude
    offset *= 0.5f;
  }

  // generate render data
  generateRenderData();

  // set state
  m_state = STATE_IDLE;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Clears object. */
void LightningEffectStrips::clear()
{
  m_beams.clear();

  // set state
  m_state = STATE_NONE;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets maximum segment midpoint offset. */
void LightningEffectStrips::setMaxSegmentOffset(float32 offset)
{
  m_maximumOffset = offset;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Generates render data based on segments. */
void LightningEffectStrips::generateRenderData()
{
  // go thru all beams
  for (BeamList::iterator it = m_beams.begin(); it != m_beams.end(); ++it)
  {
    Beam& beam = *it;

    // setup vertex declaration
    VertexDeclaration vertexDeclaration;
    vertexDeclaration.addElement(NVertexBuffer::VES_POSITION_XY);
    vertexDeclaration.addElement(NVertexBuffer::VES_TEXTURE_UV);
    vertexDeclaration.addElement(NVertexBuffer::VES_COLOR_RGBA);

    // create render data
    beam.renderData = ege_new RenderComponent(app(), "lightning-effect-lines", vertexDeclaration, m_renderPriority, EGEGraphics::RPT_TRIANGLE_STRIPS);
    beam.renderData->setMaterial(m_material);

    Vector2f startPosNegative;
    Vector2f startPosPositive;
    Vector2f endPosNegative;
    Vector2f endPosPositive;

    // NOTE: Required number of vertices: 2 + segments * 2
    //       2 for initial points of first segment
    //       2 per segments for the last 2 vertices
    float32* data = reinterpret_cast<float32*>(beam.renderData->vertexBuffer()->lock(0, 2 + beam.segments.size() * 2));

    // go thru all segments
    u32 i = 0;
    for (SegmentList::iterator itSegment = beam.segments.begin(); itSegment != beam.segments.end(); ++itSegment, ++i)
    {
      Segment& segment = *itSegment;

      bool isFirst = (i == 0);
      bool isLast  = (i == beam.segments.size() - 1);

      // check if first segment
      if (isFirst)
      {
        // generate start vertices
        startPosNegative = segment.start - segment.normal * m_width * segment.intensity;
        startPosPositive = segment.start + segment.normal * m_width * segment.intensity;

        // first 2 vertices
        *data++ = startPosNegative.x;
        *data++ = startPosNegative.y;
        *data++ = 0.0f;
        *data++ = 0.0f;
        *data++ = 1.0f;
        *data++ = 1.0f;
        *data++ = 1.0f;
        *data++ = segment.intensity;

        *data++ = startPosPositive.x;
        *data++ = startPosPositive.y;
        *data++ = 0.0f;
        *data++ = 1.0f;
        *data++ = 1.0f;
        *data++ = 1.0f;
        *data++ = 1.0f;
        *data++ = segment.intensity;
      }
      else
      {
        // if this is begining of segment other than first one, start points are exactly the same as last segment's end points
        startPosNegative = endPosNegative;
        startPosPositive = endPosPositive;
      }

      // generate end points
      endPosNegative = segment.end - segment.normal * m_width * segment.intensity;
      endPosPositive = segment.end + segment.normal * m_width * segment.intensity;

      // check if there is at least 1 more segment
      if (!isLast)
      {
        SegmentList::const_iterator itNextSegment = itSegment;
        ++itNextSegment;
        const Segment* nextSegment = &(*itNextSegment);

        float32 absDot = segment.normal.dotProduct(nextSegment->normal);
        if (absDot < -0.9f)
        {
          // store intersection points as new end points
          //endPosNegative = nextSegment->start + nextSegment->normal * m_width * nextSegment->intensity;
          //endPosPositive = nextSegment->start - nextSegment->normal * m_width * nextSegment->intensity;
        }
        else
        {
          // calculate next segments' points
          Vector2f nextStartPosNegative = nextSegment->start - nextSegment->normal * m_width * nextSegment->intensity;
          Vector2f nextStartPosPositive = nextSegment->start + nextSegment->normal * m_width * nextSegment->intensity;
          Vector2f nextEndPosNegative   = nextSegment->end - nextSegment->normal * m_width * nextSegment->intensity;
          Vector2f nextEndPosPositive   = nextSegment->end + nextSegment->normal * m_width * nextSegment->intensity;

          // find out intersection point between lines formed by negative edges of current and next segment
          Vector2f outNegative;
          Math::LineLineIntersectPoint(&outNegative, &startPosNegative, &endPosNegative, &nextStartPosNegative, &nextEndPosNegative);

          // find out intersection point between lines formed by positive edges of current and next segment
          Vector2f outPositive;
          Math::LineLineIntersectPoint(&outPositive, &startPosPositive, &endPosPositive, &nextStartPosPositive, &nextEndPosPositive);

          // store intersection points as new end points
          endPosNegative = outNegative;
          endPosPositive = outPositive;
        }
      }

      if (endPosNegative.x < 0 || endPosNegative.y < 0 || startPosPositive.x < 0 || startPosNegative.x < 0)
      {
        int a = 1;
      }

      // last 2 points
      *data++ = endPosNegative.x;
      *data++ = endPosNegative.y;
      *data++ = 1.0f;
      *data++ = 0.0f;
      *data++ = 1.0f;
      *data++ = 1.0f;
      *data++ = 1.0f;
      *data++ = segment.intensity;

      *data++ = endPosPositive.x;
      *data++ = endPosPositive.y;
      *data++ = 1.0f;
      *data++ = 1.0f;
      *data++ = 1.0f;
      *data++ = 1.0f;
      *data++ = 1.0f;
      *data++ = segment.intensity;

      // store end points normal for further update purposes
      segment.randomizationNormal = (endPosNegative - endPosPositive).normalized();
      segment.randomization = 0.0f;
    }

    beam.renderData->vertexBuffer()->unlock(data - 1);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Set offshot angle. */
void LightningEffectStrips::setOffshotAngle(const Angle& angle)
{
  m_offshotAngle = angle;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Set offshot angle variance. */
void LightningEffectStrips::setOffshotAngleVariance(const Angle& angle)
{
  m_offshotAngleVariance = angle;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Starts effect. */
void LightningEffectStrips::start()
{
  m_fadeTime = 0.0f;
  m_randomizationTime = m_randomizationInterval;

  // set state
  m_state = STATE_BUSY;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets width. */
void LightningEffectStrips::setWidth(float32 width)
{
  m_width = width;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets material. */
void LightningEffectStrips::setMaterial(const PMaterial& material)
{
  m_material = material;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets render priority. */
void LightningEffectStrips::setRenderPriority(s32 priority)
{
  m_renderPriority = priority;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets randmization variance. */
void LightningEffectStrips::setRandmizationVariance(float32 variance)
{
  m_randomizationVariance = variance;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets randomization interval. */
void LightningEffectStrips::setRandomizationInterval(const Time& time)
{
  m_randomizationInterval = time;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
