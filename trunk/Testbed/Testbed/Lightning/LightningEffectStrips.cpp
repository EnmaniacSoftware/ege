#include <EGEApplication.h>
#include <EGEMath.h>
#include <EGEDebug.h>
#include <EGEResources.h>
#include "LightningEffectStrips.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(LightningEffectStrips)
EGE_DEFINE_DELETE_OPERATORS(LightningEffectStrips)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
LightningEffectStrips::LightningEffectStrips(Application* app) : m_app(app),
                                                               m_maximumOffset(60.0f),
                                                               m_offshotAngle(EGEMath::PI * 0.25f),
                                                               m_offshotAngleVariance(EGEMath::PI * 0.125f),
                                                               m_state(STATE_NONE),
                                                               m_width(1.0f)
{
  // create render data
  //m_renderData = ege_new RenderComponent(app, "lightning-effect-lines", EGEGraphics::RP_MAIN, EGEGraphics::RPT_TRIANGLE_STRIPS);
  //if (m_renderData)
  //{
  //  m_renderData->indexBuffer()->setIndexSize(EGEIndexBuffer::IS_16BIT);
  //  if (!m_renderData->vertexBuffer()->setSemantics(EGEVertexBuffer::ST_V2_T2_C4))
  //  {
  //    // error!
  //    m_renderData = NULL;
  //  }
  //}
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
  return true;//(NULL != m_renderData);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Renders object. */
void LightningEffectStrips::render(Renderer* renderer)
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
      m_fadeTime = 1.0f;
      m_state = STATE_IDLE;
    }

    const float32 maxDisplacement = 10.0f;

    // go thru all beams
    for (BeamList::const_iterator it = m_beams.begin(); it != m_beams.end(); ++it)
    {
      const Beam& beam = *it;

      float32* data = reinterpret_cast<float32*>(beam.renderData->vertexBuffer()->lock(0, beam.segments.size() * 6 + 2));

      // go thru all segments
      int i = 0;
      for (SegmentList::const_iterator itSegment = beam.segments.begin(); itSegment != beam.segments.end(); ++itSegment, ++i)
      {
        const Segment& segment = *itSegment;

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

      beam.renderData->vertexBuffer()->unlock();
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Generates segments. */
void LightningEffectStrips::create(const Vector2f& start, const Vector2f end, s32 steps, bool allowOffshots)
{
  m_beams.clear();
  m_beams.push_back(Beam());

  Segment segment;
  segment.start         = start;
  segment.end           = end;
  segment.normal        = (segment.end - segment.start).perpendicular().normalized();
  segment.intensity     = 1.0f;
  m_beams.back().segments.push_back(segment);

  float32 offset = 60;
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
        if (m_random() & 0x1)
        {
          offsetVector *= -1.0f;
        }

        // move mid point along the perpendicular vectors
        midPoint += offsetVector * offset;

        // create new segment for first subsegment
        segment.start     = oldSegment.start;
        segment.end       = midPoint;
        segment.normal    = (segment.end - segment.start).perpendicular().normalized();
        segment.intensity = oldSegment.intensity;

        // update current segment to second subsegment
        oldSegment.start = midPoint;
        oldSegment.end  = oldSegment.end;
        oldSegment.normal = (oldSegment.end - midPoint).perpendicular().normalized();

        beam.segments.insert(itSegment, segment);

        // check if offshot should be generated
        if (allowOffshots)
        {
          if ((m_random() % 100) < oldSegment.intensity * 50)
          {
            Beam offshotBeam;

            segment.start = midPoint;

            Vector2f direction = oldSegment.end - midPoint;
            float32 dirLength = direction.length();
            direction.normalize();

            Angle angle = Angle::FromDegrees(m_random(5, 10));

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
  const float32 beginEndSizeCoe = 0.2f;

  // go thru all beams
  for (BeamList::iterator it = m_beams.begin(); it != m_beams.end(); ++it)
  {
    Beam& beam = *it;

    // create render data
    beam.renderData = ege_new RenderComponent(m_app, "lightning-effect-lines", EGEGraphics::RP_MAIN, EGEGraphics::RPT_TRIANGLE_STRIPS);
    if (!beam.renderData->vertexBuffer()->setSemantics(EGEVertexBuffer::ST_V2_T2_C4))
    {
      // error!
      return;
    }

    beam.renderData->setMaterial(m_material);

    float32* data = reinterpret_cast<float32*>(beam.renderData->vertexBuffer()->lock(0, beam.segments.size() * 6 + 2));

    // go thru all segments
    int i = 0;
    for (SegmentList::const_iterator itSegment = beam.segments.begin(); itSegment != beam.segments.end(); ++itSegment, ++i)
    {
      const Segment& segment = *itSegment;

      bool isFirst = (i == 0);
      bool isLast  = (i == beam.segments.size() - 1);

      if (isFirst)
      {
        *data++ = Math::Lerp(segment.start.x, segment.end.x, 0) - segment.normal.x * m_width * segment.intensity;
        *data++ = Math::Lerp(segment.start.y, segment.end.y, 0) - segment.normal.y * m_width * segment.intensity;
        *data++ = (0 == i) ? 0.0f : 0.25f;
        *data++ = 0.0f;
        *data++ = 1.0f;
        *data++ = 1.0f;
        *data++ = 1.0f;
        *data++ = segment.intensity;

        *data++ = Math::Lerp(segment.start.x, segment.end.x, 0) + segment.normal.x * m_width * segment.intensity;
        *data++ = Math::Lerp(segment.start.y, segment.end.y, 0) + segment.normal.y * m_width * segment.intensity;
        *data++ = (0 == i) ? 0.0f : 0.25f;
        *data++ = 1.0f;
        *data++ = 1.0f;
        *data++ = 1.0f;
        *data++ = 1.0f;
        *data++ = segment.intensity;
      }

      *data++ = Math::Lerp(segment.start.x, segment.end.x, beginEndSizeCoe) - segment.normal.x * m_width * segment.intensity;
      *data++ = Math::Lerp(segment.start.y, segment.end.y, beginEndSizeCoe) - segment.normal.y * m_width * segment.intensity;
      *data++ = 0.25f;
      *data++ = 0.0f;
      *data++ = 1.0f;
      *data++ = 1.0f;
      *data++ = 1.0f;
      *data++ = segment.intensity;

      *data++ = Math::Lerp(segment.start.x, segment.end.x, beginEndSizeCoe) + segment.normal.x * m_width * segment.intensity;
      *data++ = Math::Lerp(segment.start.y, segment.end.y, beginEndSizeCoe) + segment.normal.y * m_width * segment.intensity;
      *data++ = 0.25f;
      *data++ = 1.0f;
      *data++ = 1.0f;
      *data++ = 1.0f;
      *data++ = 1.0f;
      *data++ = segment.intensity;

      *data++ = Math::Lerp(segment.start.x, segment.end.x, 1.0f - beginEndSizeCoe) - segment.normal.x * m_width * segment.intensity;
      *data++ = Math::Lerp(segment.start.y, segment.end.y, 1.0f - beginEndSizeCoe) - segment.normal.y * m_width * segment.intensity;
      *data++ = 0.75f;
      *data++ = 0.0f;
      *data++ = 1.0f;
      *data++ = 1.0f;
      *data++ = 1.0f;
      *data++ = segment.intensity;

      *data++ = Math::Lerp(segment.start.x, segment.end.x, 1.0f - beginEndSizeCoe) + segment.normal.x * m_width * segment.intensity;
      *data++ = Math::Lerp(segment.start.y, segment.end.y, 1.0f - beginEndSizeCoe) + segment.normal.y * m_width * segment.intensity;
      *data++ = 0.75f;
      *data++ = 1.0f;
      *data++ = 1.0f;
      *data++ = 1.0f;
      *data++ = 1.0f;
      *data++ = segment.intensity;

      *data++ = Math::Lerp(segment.start.x, segment.end.x, 1.0f) - segment.normal.x * m_width * segment.intensity;
      *data++ = Math::Lerp(segment.start.y, segment.end.y, 1.0f) - segment.normal.y * m_width * segment.intensity;
      *data++ = isLast ? 1.0f : 0.75f;
      *data++ = 0.0f;
      *data++ = 1.0f;
      *data++ = 1.0f;
      *data++ = 1.0f;
      *data++ = segment.intensity;

      *data++ = Math::Lerp(segment.start.x, segment.end.x, 1.0f) + segment.normal.x * m_width * segment.intensity;
      *data++ = Math::Lerp(segment.start.y, segment.end.y, 1.0f) + segment.normal.y * m_width * segment.intensity;
      *data++ = isLast ? 1.0f : 0.75f;
      *data++ = 1.0f;
      *data++ = 1.0f;
      *data++ = 1.0f;
      *data++ = 1.0f;
      *data++ = segment.intensity;
    }

    beam.renderData->vertexBuffer()->unlock();
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
