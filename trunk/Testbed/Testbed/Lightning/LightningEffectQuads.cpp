#include <EGEApplication.h>
#include <EGEMath.h>
#include <EGEDebug.h>
#include <EGEResources.h>
#include "LightningEffectQuads.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(LightningEffectQuads)
EGE_DEFINE_DELETE_OPERATORS(LightningEffectQuads)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
LightningEffectQuads::LightningEffectQuads(Application* app) : m_app(app),
                                                               m_maximumOffset(60.0f),
                                                               m_offshotAngle(EGEMath::PI * 0.25f),
                                                               m_offshotAngleVariance(EGEMath::PI * 0.125f),
                                                               m_state(STATE_NONE)
{
  // create render data
  m_renderData = ege_new RenderComponent(app, "lightning-effect-lines", EGEGraphics::RP_MAIN, EGEGraphics::RPT_TRIANGLES);
  if (m_renderData)
  {
    m_renderData->indexBuffer()->setIndexSize(EGEIndexBuffer::IS_16BIT);
    if (!m_renderData->vertexBuffer()->setSemantics(EGEVertexBuffer::ST_V2_T2_C4))
    {
      // error!
      m_renderData = NULL;
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
LightningEffectQuads::~LightningEffectQuads()
{
  clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool LightningEffectQuads::isValid() const
{
  return (NULL != m_renderData);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Renders object. */
void LightningEffectQuads::render(Renderer* renderer)
{
  if (STATE_BUSY == m_state)
  {
    renderer->addForRendering(m_renderData);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates effect. */
void LightningEffectQuads::update(const Time& time)
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

    float32* data = reinterpret_cast<float32*>(m_renderData->vertexBuffer()->lock(0, m_segments.size() * 8));

    // go thru all segments
    for (SegmentList::const_iterator it = m_segments.begin(); it != m_segments.end(); ++it)
    {
      const Segment segment= **it;

      *data++;// = segment.start.x + segment.normal.x * width * segment.intensity;
      *data++;// = segment.start.y + segment.normal.y * width * segment.intensity;
      *data++;// = 0;
      *data++;// = 0;
      *data++;// = 1;
      *data++;// = 1;
      *data++;// = 1;
      *data++ = segment.intensity * (1.0f - m_fadeTime.seconds());

      *data++;// = segment.start.x - segment.normal.x * width * segment.intensity;
      *data++;// = segment.start.y - segment.normal.y * width * segment.intensity;
      *data++;// = 0;
      *data++;// = 1;
      *data++;// = 1;
      *data++;// = 1;
      *data++;// = 1;
      *data++ = segment.intensity * (1.0f - m_fadeTime.seconds());

      *data++;//= segment.end.x + segment.normal.x * width * segment.intensity;
      *data++;//= segment.end.y + segment.normal.y * width * segment.intensity;
      *data++;//= 1;
      *data++;//= 0;
      *data++;//= 1;
      *data++;//= 1;
      *data++;//= 1;
      *data++ = segment.intensity * (1.0f - m_fadeTime.seconds());

      *data++;//= segment.end.x - segment.normal.x * width * segment.intensity;
      *data++;//= segment.end.y - segment.normal.y * width * segment.intensity;
      *data++;//= 1;
      *data++;//= 1;
      *data++;//= 1;
      *data++;//= 1;
      *data++;//= 1;
      *data++ = segment.intensity * (1.0f - m_fadeTime.seconds());

      *data++;// = segment.start.x + segment.normal.x * width * segment.intensity;
      *data++;// = segment.start.y + segment.normal.y * width * segment.intensity;
      *data++;// = 0;
      *data++;// = 0;
      *data++;// = 1;
      *data++;// = 1;
      *data++;// = 1;
      *data++ = segment.intensity * (1.0f - m_fadeTime.seconds());

      *data++;// = segment.start.x - segment.normal.x * width * segment.intensity;
      *data++;// = segment.start.y - segment.normal.y * width * segment.intensity;
      *data++;// = 0;
      *data++;// = 1;
      *data++;// = 1;
      *data++;// = 1;
      *data++;// = 1;
      *data++ = segment.intensity * (1.0f - m_fadeTime.seconds());

      *data++;//= segment.end.x + segment.normal.x * width * segment.intensity;
      *data++;//= segment.end.y + segment.normal.y * width * segment.intensity;
      *data++;//= 1;
      *data++;//= 0;
      *data++;//= 1;
      *data++;//= 1;
      *data++;//= 1;
      *data++ = segment.intensity * (1.0f - m_fadeTime.seconds());

      *data++;//= segment.end.x - segment.normal.x * width * segment.intensity;
      *data++;//= segment.end.y - segment.normal.y * width * segment.intensity;
      *data++;//= 1;
      *data++;//= 1;
      *data++;//= 1;
      *data++;//= 1;
      *data++;//= 1;
      *data++ = segment.intensity * (1.0f - m_fadeTime.seconds());
    }

    m_renderData->vertexBuffer()->unlock();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Generates segments. */
void LightningEffectQuads::create(const Vector2f& start, const Vector2f end, s32 steps, bool allowOffshots)
{
  // cleanup
  clear();

  // create first segment
  Segment* segment       = ege_new Segment();
  segment->start         = start;
  segment->end           = end;
  segment->begining      = true;
  segment->ending        = true;
  segment->normal        = (end - start).perpendicular().normalized();
  segment->intensity     = 1.0f;
  m_segments.push_back(segment);

  float32 offset = m_maximumOffset;
  for (s32 step = 0; step < steps; ++step)
  {
    // go thru all segments
    s32 count = m_segments.size();
    for (SegmentList::iterator it = m_segments.begin(); count > 0; ++it, --count)
    {
      Segment oldSegment = **it;

      // determine segment vector
      Vector2f segmentVector = oldSegment.end - oldSegment.start;

      // get point in the middle of segment
      Vector2f midPoint(oldSegment.start.x + segmentVector.x * 0.5f, oldSegment.start.y + segmentVector.y * 0.5f);

      // calulate normalized vector perpendicular to current segment
      Vector2f offsetVector = segment->normal;
      if (m_random() & 0x1)
      {
        offsetVector *= -1.0f;
      }

      // move mid point along the perpendicular vectors
      midPoint += offsetVector * offset;

      // update current segment to second subsegment
      (*it)->start = midPoint;
      (*it)->end  = oldSegment.end;
      (*it)->normal = (oldSegment.end - midPoint).perpendicular().normalized();
      (*it)->begining = false;

      // create new segment for first subsegment
      Segment* newSegment = ege_new Segment();
      newSegment->start     = oldSegment.start;
      newSegment->end       = midPoint;
      newSegment->normal    = (newSegment->end - newSegment->start).perpendicular().normalized();
      newSegment->intensity = oldSegment.intensity;
      newSegment->begining  = oldSegment.begining;
      newSegment->ending    = false;

      (*it)->prev = newSegment;
      newSegment->next = *it;
      newSegment->prev = oldSegment.prev;

      if (oldSegment.prev)
      {
        oldSegment.prev->next = newSegment;
      }

      m_segments.insert(it, newSegment);

      // check if offshot should be generated
      if (allowOffshots)
      {
        if (((m_random() % 100) < oldSegment.intensity * 50) && (NULL == (*it)->prev->offshot))
        {
          Segment* newSegment = ege_new Segment();

          newSegment->start = midPoint;

          Vector2f direction = midPoint - (*it)->prev->start;
          float32 dirLength = direction.length();
          direction.normalize();

          Angle angle = m_offshotAngle + m_offshotAngleVariance * m_random(-1.0f, 1.0f);

          float32 cos = Math::Cos(angle.radians());
          float32 sin = Math::Sin(angle.radians());

          newSegment->end.x = direction.x * cos - direction.y * sin;
          newSegment->end.y = direction.x * sin + direction.y * cos;

          newSegment->end       = newSegment->end * dirLength * 0.7f + midPoint;
          newSegment->normal    = (newSegment->end - newSegment->start).perpendicular().normalized();
          newSegment->begining  = false;
          newSegment->ending    = true;
          newSegment->prev      = (*it)->prev;

          (*it)->prev->offshot = newSegment;

          newSegment->intensity = oldSegment.intensity * 0.5f;
          m_segments.push_back(newSegment);
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
void LightningEffectQuads::clear()
{
  // cleanup
  for (SegmentList::iterator it = m_segments.begin(); it != m_segments.end(); ++it)
  {
    EGE_DELETE(*it);
  }
  m_segments.clear();

  // set state
  m_state = STATE_NONE;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets maximum segment midpoint offset. */
void LightningEffectQuads::setSegmentOffset(float32 offset)
{
  m_maximumOffset = offset;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Generates render data based on segments. */
void LightningEffectQuads::generateRenderData()
{
  const float32 width = 1.0f;
  const float32 beginEndSizeCoe = 0.2f;

  float32* data = reinterpret_cast<float32*>(m_renderData->vertexBuffer()->lock(0, m_segments.size() * 8));
  s16* index    = reinterpret_cast<s16*>(m_renderData->indexBuffer()->lock(0, m_segments.size() * 6 * 3));

  Vector2f normal;

  // go thru all segments
  int i = 0;
  for (SegmentList::const_iterator it = m_segments.begin(); it != m_segments.end(); ++it, ++i)
  {
    const Segment* segment = *it;

    // begining quad
    *index++ = i * 8 + 0;
    *index++ = i * 8 + 1;
    *index++ = i * 8 + 2;
    *index++ = i * 8 + 1;
    *index++ = i * 8 + 2;
    *index++ = i * 8 + 3;

    // middle quad
    *index++ = i * 8 + 2;
    *index++ = i * 8 + 3;
    *index++ = i * 8 + 4;
    *index++ = i * 8 + 3;
    *index++ = i * 8 + 4;
    *index++ = i * 8 + 5;

    // ending quad
    *index++ = i * 8 + 4;
    *index++ = i * 8 + 5;
    *index++ = i * 8 + 6;
    *index++ = i * 8 + 5;
    *index++ = i * 8 + 6;
    *index++ = i * 8 + 7;

    // check if current segment is an offshot segment
    // NOTE: this happens if previous segment's offshot segment is current segment
    bool isOffshot = segment->prev && (segment->prev->offshot == segment);

    // begining quad
    if (segment->prev && !isOffshot)
    {
      normal = (segment->prev->normal + segment->normal).normalized();
    }
    else
    {
      normal = segment->normal;
    }

    *data++ = Math::Lerp(segment->start.x, segment->end.x, 0) + normal.x * width * segment->intensity;
    *data++ = Math::Lerp(segment->start.y, segment->end.y, 0) + normal.y * width * segment->intensity;
    *data++ = segment->begining ? 0.0f : 0.25f;
    *data++ = 0.0f;
    *data++ = 1.0f;
    *data++ = 1.0f;
    *data++ = 1.0f;
    *data++ = segment->intensity;

    *data++ = Math::Lerp(segment->start.x, segment->end.x, 0) - normal.x * width * segment->intensity;
    *data++ = Math::Lerp(segment->start.y, segment->end.y, 0) - normal.y * width * segment->intensity;
    *data++ = segment->begining ? 0.0f : 0.25f;
    *data++ = 1.0f;
    *data++ = 1.0f;
    *data++ = 1.0f;
    *data++ = 1.0f;
    *data++ = segment->intensity;

    // end of begining quad and begining of middle quad
    *data++ = Math::Lerp(segment->start.x, segment->end.x, beginEndSizeCoe) + segment->normal.x * width * segment->intensity;
    *data++ = Math::Lerp(segment->start.y, segment->end.y, beginEndSizeCoe) + segment->normal.y * width * segment->intensity;
    *data++ = 0.25f;
    *data++ = 0.0f;
    *data++ = 1.0f;
    *data++ = 1.0f;
    *data++ = 1.0f;
    *data++ = segment->intensity;

    *data++ = Math::Lerp(segment->start.x, segment->end.x, beginEndSizeCoe) - segment->normal.x * width * segment->intensity;
    *data++ = Math::Lerp(segment->start.y, segment->end.y, beginEndSizeCoe) - segment->normal.y * width * segment->intensity;
    *data++ = 0.25f;
    *data++ = 1.0f;
    *data++ = 1.0f;
    *data++ = 1.0f;
    *data++ = 1.0f;
    *data++ = segment->intensity;

    // end of middle quad and begining of end quad
    *data++ = Math::Lerp(segment->start.x, segment->end.x, 1.0f - beginEndSizeCoe) + segment->normal.x * width * segment->intensity;
    *data++ = Math::Lerp(segment->start.y, segment->end.y, 1.0f - beginEndSizeCoe) + segment->normal.y * width * segment->intensity;
    *data++ = 0.75f;
    *data++ = 0.0f;
    *data++ = 1.0f;
    *data++ = 1.0f;
    *data++ = 1.0f;
    *data++ = segment->intensity;

    *data++ = Math::Lerp(segment->start.x, segment->end.x, 1.0f - beginEndSizeCoe) - segment->normal.x * width * segment->intensity;
    *data++ = Math::Lerp(segment->start.y, segment->end.y, 1.0f - beginEndSizeCoe) - segment->normal.y * width * segment->intensity;
    *data++ = 0.75f;
    *data++ = 1.0f;
    *data++ = 1.0f;
    *data++ = 1.0f;
    *data++ = 1.0f;
    *data++ = segment->intensity;

    if (segment->next)
    {
      normal = (segment->next->normal + segment->normal).normalized();
    }
    else
    {
      normal = segment->normal;
    }

    // end of ending quad
    *data++ = Math::Lerp(segment->start.x, segment->end.x, 1.0f) + normal.x * width * segment->intensity;
    *data++ = Math::Lerp(segment->start.y, segment->end.y, 1.0f) + normal.y * width * segment->intensity;
    *data++ = segment->ending ? 1.0f : 0.75f;
    *data++ = 0.0f;
    *data++ = 1.0f;
    *data++ = 1.0f;
    *data++ = 1.0f;
    *data++ = segment->intensity;

    *data++ = Math::Lerp(segment->start.x, segment->end.x, 1.0f) - normal.x * width * segment->intensity;
    *data++ = Math::Lerp(segment->start.y, segment->end.y, 1.0f) - normal.y * width * segment->intensity;
    *data++ = segment->ending ? 1.0f : 0.75f;
    *data++ = 1.0f;
    *data++ = 1.0f;
    *data++ = 1.0f;
    *data++ = 1.0f;
    *data++ = segment->intensity;
  }

  m_renderData->vertexBuffer()->unlock();
  m_renderData->indexBuffer()->unlock();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Set offshot angle. */
void LightningEffectQuads::setOffshotAngle(const Angle& angle)
{
  m_offshotAngle = angle;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Set offshot angle variance. */
void LightningEffectQuads::setOffshotAngleVariance(const Angle& angle)
{
  m_offshotAngleVariance = angle;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Starts effect. */
void LightningEffectQuads::start()
{
  m_fadeTime = 0.0f;

  // set state
  m_state = STATE_BUSY;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
