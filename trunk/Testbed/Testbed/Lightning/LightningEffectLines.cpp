#include "LightningEffectLines.h"
#include <EGEMath.h>
#include <EGEDebug.h>
#include <EGEResources.h>
#include <EGEOverlay.h>
#include <EGEApplication.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
LightningEffectLines::LightningEffectLines(Application* app) : SceneNodeObject("lightning-effect-lines"), 
                                                               m_app(app)
{
  m_renderData = ege_new RenderComponent(app, "lightning-effect-lines", EGEGraphics::RP_MAIN, EGEGraphics::RPT_LINES);
  if (m_renderData)
  {
    m_renderData->vertexBuffer()->setSemantics(EGEVertexBuffer::ST_V2_C4);
    m_renderData->setLineWidth(2.0f);

    PMaterial material = ege_new Material(app);
    RenderPass* pass = material->addPass(NULL);
    m_renderData->setMaterial(material);
    material->setSrcBlendFactor(EGEGraphics::BF_SRC_ALPHA);
    material->setDstBlendFactor(EGEGraphics::BF_ONE_MINUS_SRC_ALPHA);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
LightningEffectLines::~LightningEffectLines()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! SceneNodeObject override. Adds object render data for rendering with given renderer. */
bool LightningEffectLines::addForRendering(Renderer* renderer, const Matrix4f& transform)
{
  if (m_fadeTime.seconds() < 1.0f)
  {
    renderer->addForRendering(m_renderData, parentNode()->worldMatrix().multiply(transform));
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates effect. */
void LightningEffectLines::update(const Time& time)
{
  m_fadeTime += time * 2.0f;
  if (m_fadeTime.seconds() > 0.5f)
  {
    m_fadeTime = 0.5f;
  }

  float32* data = (float32*) m_renderData->vertexBuffer()->lock(0, m_segments.size() * 2);
  for (SegmentList::const_iterator it = m_segments.begin(); it != m_segments.end(); ++it)
  {
    const Segment& segment= *it;

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
    *data++ = segment.intensity * (1.0f - m_fadeTime.seconds());
  }
  m_renderData->vertexBuffer()->unlock();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Pointer event receiver. */
void LightningEffectLines::pointerEvent(PPointerData data)
{
  bool color = false;

  if (EGEInput::ACTION_BUTTON_DOWN == data->action())
  {
    m_fadeTime = 0.0f;

    create(Vector2f(100, 50), Vector2f(600, 250), 5, true);

    float32* data = (float32*) m_renderData->vertexBuffer()->lock(0, m_segments.size() * 2);
    for (SegmentList::const_iterator it = m_segments.begin(); it != m_segments.end(); ++it)
    {
      *data++ = (*it).start.x;
      *data++ = (*it).start.y + 150;
      *data++ = 1;
      *data++ = 1;
      *data++ = 1;
      *data++ = (*it).intensity;

      *data++ = (*it).end.x;
      *data++ = (*it).end.y + 150;
      *data++ = 1;
      *data++ = 1;
      *data++ = 1;
      *data++ = (*it).intensity;
    }

    m_renderData->vertexBuffer()->unlock();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Generates segments. */
void LightningEffectLines::create(const Vector2f& start, const Vector2f end, s32 steps, bool allowOffshots)
{
  m_segments.clear();

  Segment segment;
  segment.start         = start;
  segment.end           = end;
  segment.intensity     = 1.0f;
  m_segments.push_back(segment);

  float32 offset = 60;
  for (s32 step = 0; step < steps; ++step)
  {
    // go thru all segments
    for (SegmentList::iterator it = m_segments.begin(); it != m_segments.end(); ++it)
    {
      Segment oldSegment = *it;

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

      // update current segment to second subsegment
      (*it).start = midPoint;
      (*it).end  = oldSegment.end;
      (*it).normal = (oldSegment.end - midPoint).perpendicular().normalized();

      // create new segment for first subsegment
      segment.start     = oldSegment.start;
      segment.end       = midPoint;
      segment.normal    = (segment.end - segment.start).perpendicular().normalized();
      segment.intensity = oldSegment.intensity;

      m_segments.insert(it, segment);

      // check if offshot should be generated
      if (allowOffshots)
      {
        if ((m_random() % 100) < oldSegment.intensity * 50)
        {
          segment.start = midPoint;

          Vector2f direction = midPoint - oldSegment.start;
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
          m_segments.push_front(segment);
        }
      }
    }

    // update offset magnitude
    offset *= 0.5f;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
