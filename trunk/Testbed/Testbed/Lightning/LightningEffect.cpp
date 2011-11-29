#include "LightningEffect.h"
#include <EGEMath.h>
#include <EGEDebug.h>
#include <EGEResources.h>
#include <EGEOverlay.h>
#include <EGEApplication.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
LightningEffect::LightningEffect(Application* app) : SceneNodeObject("lightning-effect"), 
                                                     m_app(app)
{
  m_renderData = ege_new RenderComponent(app, "lightning-effect-lines", EGEGraphics::RP_MAIN, EGEGraphics::RPT_LINES);
  m_renderDataQuad = ege_new RenderComponent(app, "lightning-effect-lines-1", EGEGraphics::RP_MAIN + 1, EGEGraphics::RPT_TRIANGLES);
  if (m_renderData)
  {
    m_renderData->vertexBuffer()->setSemantics(EGEVertexBuffer::ST_V2_C4);
    m_renderDataQuad->vertexBuffer()->setSemantics(EGEVertexBuffer::ST_V2_T2_C4);
    m_renderData->setLineWidth(2.0f);

    PMaterial material = ege_new Material(app);
    RenderPass* pass = material->addPass(NULL);
    m_renderData->setMaterial(material);
    material->setSrcBlendFactor(EGEGraphics::BF_SRC_ALPHA);
    material->setDstBlendFactor(EGEGraphics::BF_ONE_MINUS_SRC_ALPHA);

    PResourceMaterial resource = app->resourceManager()->resource(RESOURCE_NAME_MATERIAL, "beam");
    m_renderDataQuad->setMaterial(resource->createInstance());
  }

  m_renderDataQuad->indexBuffer()->create(IndexBuffer::SIZE_16BIT, 2);
  
  PResourceFont fontResource = app->resourceManager()->resource(RESOURCE_NAME_FONT, "debug-font");
  if (fontResource)
  {
    PTextOverlay overlay = ege_new TextOverlay(app, "buhaha");
    overlay->setFont(fontResource->font());
    app->overlayManager()->add(overlay);
    overlay->physics()->setPosition(Vector4f(0, 80, 0));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
LightningEffect::~LightningEffect()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! SceneNodeObject override. Adds object render data for rendering with given renderer. */
bool LightningEffect::addForRendering(Renderer* renderer)
{
  renderer->addForRendering(parentNode()->worldMatrix(), m_renderData);
  renderer->addForRendering(parentNode()->worldMatrix(), m_renderDataQuad);

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates effect. */
void LightningEffect::update(const Time& time)
{
  //ege_cast<TextOverlay*>(m_app->overlayManager()->overlay("buhaha"))->setText(Text::Format("%d", a));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Pointer event receiver. */
void LightningEffect::pointerEvent(PPointerData data)
{
  if (EGEInput::ACTION_BUTTON_DOWN == data->action())
  {
    List<Segment> list = generateSegments(Vector2f(100, 100), Vector2f(700, 500));

    float32* data = (float32*) m_renderData->vertexBuffer()->lock(0, list.size() * 2);
    for (List<Segment>::const_iterator it = list.begin(); it != list.end(); ++it)
    {
      *data++ = (*it).start.x;
      *data++ = (*it).start.y;
      *data++ = 1;
      *data++ = 1;
      *data++ = 1;
      *data++ = (*it).intensity;

      *data++ = (*it).end.x;
      *data++ = (*it).end.y;
      *data++ = 1;
      *data++ = 1;
      *data++ = 1;
      *data++ = (*it).intensity;
    }

    m_renderData->vertexBuffer()->unlock();

    {
      float32* data = (float32*) m_renderDataQuad->vertexBuffer()->lock(0, list.size() * 4);
      s16* index = (s16*) m_renderDataQuad->indexBuffer()->lock(0, list.size() * 6);

      const float32 width = 10.0f;

      int i = 0;
      for (List<Segment>::const_iterator it = list.begin(); it != list.end(); ++it, ++i)
      {
        const Segment& segment= *it;

        *index++ = i * 4 + 0;
        *index++ = i * 4 + 1;
        *index++ = i * 4 + 2;
        *index++ = i * 4 + 1;
        *index++ = i * 4 + 2;
        *index++ = i * 4 + 3;

        *data++ = segment.start.x + segment.normal.x * width * segment.intensity;
        *data++ = segment.start.y + segment.normal.y * width * segment.intensity;
        *data++ = 0;
        *data++ = 0;
        *data++ = 1;
        *data++ = 1;
        *data++ = 1;
        *data++ = segment.intensity;

        *data++ = segment.start.x - segment.normal.x * width * segment.intensity;
        *data++ = segment.start.y - segment.normal.y * width * segment.intensity;
        *data++ = 0;
        *data++ = 1;
        *data++ = 1;
        *data++ = 1;
        *data++ = 1;
        *data++ = segment.intensity;

        *data++ = segment.end.x + segment.normal.x * width * segment.intensity;
        *data++ = segment.end.y + segment.normal.y * width * segment.intensity;
        *data++ = 1;
        *data++ = 0;
        *data++ = 1;
        *data++ = 1;
        *data++ = 1;
        *data++ = segment.intensity;

        *data++ = segment.end.x - segment.normal.x * width * segment.intensity;
        *data++ = segment.end.y - segment.normal.y * width * segment.intensity;
        *data++ = 1;
        *data++ = 1;
        *data++ = 1;
        *data++ = 1;
        *data++ = 1;
        *data++ = segment.intensity;
      }

      m_renderDataQuad->vertexBuffer()->unlock();
      m_renderDataQuad->indexBuffer()->unlock();
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Generates segments. */
EGE::List<Segment> LightningEffect::generateSegments(const Vector2f& start, const Vector2f end)
{
  List<Segment> segments;

  Segment segment;
  segment.start     = start;
  segment.end       = end;
  segment.normal    = (end - start).perpendicular().normalized();
  segment.intensity = 1.0f;
  segments.push_back(segment);

  float32 offset = 60;
  for (s32 step = 0; step < 1; ++step)
  {
    // go thru all segments
    for (List<Segment>::iterator it = segments.begin(); it != segments.end(); ++it)
    {
      Segment oldSegment = *it;

      // determine segment vector
      Vector2f segmentVector = oldSegment.end - oldSegment.start;

      // get point in the middle of segment
      Vector2f midPoint(oldSegment.start.x + segmentVector.x * 0.5f, oldSegment.start.y + segmentVector.y * 0.5f);

      // calulate normalized vector perpendicular to current segment
      Vector2f offsetVector = segment.normal; //segmentVector.perpendicular();
//      offsetVector.normalize();
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
      segments.insert(it, segment);

      // check if offshot should be generated
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

        segment.end = segment.end * dirLength * 0.7f + midPoint;
        segment.normal    = (segment.end - segment.start).perpendicular().normalized();
        segment.intensity = oldSegment.intensity * 0.5f;
        segments.push_front(segment);
      }
    }

    // update offset magnitude
    offset *= 0.5f;
  }

  return segments;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
