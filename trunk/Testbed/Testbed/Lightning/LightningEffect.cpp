#include "LightningEffect.h"
#include <EGEMath.h>
#include <EGEDebug.h>
#include <EGEResources.h>
#include <EGEOverlay.h>
#include <EGEApplication.h>
#include <gl/GL.h>

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
    m_renderData->setLineWidth(1.0f);

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

  glDisable(GL_DEPTH_TEST);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
LightningEffect::~LightningEffect()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! SceneNodeObject override. Adds object render data for rendering with given renderer. */
bool LightningEffect::addForRendering(Renderer* renderer)
{
  if (m_fadeTime.seconds() < 1.0f)
  {
    renderer->addForRendering(parentNode()->worldMatrix(), m_renderData);
    renderer->addForRendering(parentNode()->worldMatrix(), m_renderDataQuad);
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates effect. */
void LightningEffect::update(const Time& time)
{
  m_fadeTime += time * 2.0f;
  if (m_fadeTime.seconds() > 1.0f)
  {
    m_fadeTime = 1.0f;
  }

  float32* data = (float32*) m_renderData->vertexBuffer()->lock(0, m_segments.size() * 2);
  for (List<Segment>::const_iterator it = m_segments.begin(); it != m_segments.end(); ++it)
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

  data = (float32*) m_renderDataQuad->vertexBuffer()->lock(0, m_segments.size() * 4 * 3);

  for (List<Segment>::const_iterator it = m_segments.begin(); it != m_segments.end(); ++it)
  {
    const Segment& segment= *it;

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

  m_renderDataQuad->vertexBuffer()->unlock();

  //ege_cast<TextOverlay*>(m_app->overlayManager()->overlay("buhaha"))->setText(Text::Format("%d", a));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Pointer event receiver. */
void LightningEffect::pointerEvent(PPointerData data)
{
  if (EGEInput::ACTION_BUTTON_DOWN == data->action())
  {
    m_fadeTime = 0.0f;

    m_segments = generateSegments(Vector2f(100, 100), Vector2f(500, 300), 5, true);

    float32* data = (float32*) m_renderData->vertexBuffer()->lock(0, m_segments.size() * 2);
    for (List<Segment>::const_iterator it = m_segments.begin(); it != m_segments.end(); ++it)
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
      float32* data = (float32*) m_renderDataQuad->vertexBuffer()->lock(0, m_segments.size() * 4 * 3);
      s16* index = (s16*) m_renderDataQuad->indexBuffer()->lock(0, m_segments.size() * 6 * 3);

      const float32 width = 3.0f;
      const float32 beginEndSizeCoe = 0.05f;
      const float32 beginEndOffset = 0.015f;

      int i = 0;
      for (List<Segment>::const_iterator it = m_segments.begin(); it != m_segments.end(); ++it, ++i)
      {
        const Segment& segment= *it;

        // begin
        *index++ = i * 12 + 0;
        *index++ = i * 12 + 1;
        *index++ = i * 12 + 2;
        *index++ = i * 12 + 1;
        *index++ = i * 12 + 2;
        *index++ = i * 12 + 3;

        // middle
        *index++ = i * 12 + 4;
        *index++ = i * 12 + 5;
        *index++ = i * 12 + 6;
        *index++ = i * 12 + 5;
        *index++ = i * 12 + 6;
        *index++ = i * 12 + 7;

        // end
        *index++ = i * 12 + 8;
        *index++ = i * 12 + 9;
        *index++ = i * 12 + 10;
        *index++ = i * 12 + 9;
        *index++ = i * 12 + 10;
        *index++ = i * 12 + 11;

        // begin
        *data++ = Math::Lerp(segment.start.x, segment.end.x, -beginEndOffset) + segment.normal.x * width * segment.intensity;
        *data++ = Math::Lerp(segment.start.y, segment.end.y, -beginEndOffset) + segment.normal.y * width * segment.intensity;
        *data++ = 0;
        *data++ = 0;
        *data++ = 1;
        *data++ = 1;
        *data++ = 1;
        *data++ = segment.intensity;

        *data++ = Math::Lerp(segment.start.x, segment.end.x, -beginEndOffset) - segment.normal.x * width * segment.intensity;
        *data++ = Math::Lerp(segment.start.y, segment.end.y, -beginEndOffset) - segment.normal.y * width * segment.intensity;
        *data++ = 0;
        *data++ = 1;
        *data++ = 1;
        *data++ = 1;
        *data++ = 1;
        *data++ = segment.intensity;

        *data++ = Math::Lerp(segment.start.x, segment.end.x, beginEndSizeCoe - beginEndOffset) + segment.normal.x * width * segment.intensity;
        *data++ = Math::Lerp(segment.start.y, segment.end.y, beginEndSizeCoe - beginEndOffset) + segment.normal.y * width * segment.intensity;
        *data++ = 0.25f;
        *data++ = 0;
        *data++ = 1;
        *data++ = 1;
        *data++ = 1;
        *data++ = segment.intensity;

        *data++ = Math::Lerp(segment.start.x, segment.end.x, beginEndSizeCoe - beginEndOffset) - segment.normal.x * width * segment.intensity;
        *data++ = Math::Lerp(segment.start.y, segment.end.y, beginEndSizeCoe - beginEndOffset) - segment.normal.y * width * segment.intensity;
        *data++ = 0.25f;
        *data++ = 1;
        *data++ = 1;
        *data++ = 1;
        *data++ = 1;
        *data++ = segment.intensity;

        // middle
        *data++ = Math::Lerp(segment.start.x, segment.end.x, beginEndSizeCoe - beginEndOffset) + segment.normal.x * width * segment.intensity;
        *data++ = Math::Lerp(segment.start.y, segment.end.y, beginEndSizeCoe - beginEndOffset) + segment.normal.y * width * segment.intensity;
        *data++ = 0.25f;
        *data++ = 0;
        *data++ = 1;
        *data++ = 1;
        *data++ = 1;
        *data++ = segment.intensity;

        *data++ = Math::Lerp(segment.start.x, segment.end.x, beginEndSizeCoe - beginEndOffset) - segment.normal.x * width * segment.intensity;
        *data++ = Math::Lerp(segment.start.y, segment.end.y, beginEndSizeCoe - beginEndOffset) - segment.normal.y * width * segment.intensity;
        *data++ = 0.25f;
        *data++ = 1;
        *data++ = 1;
        *data++ = 1;
        *data++ = 1;
        *data++ = segment.intensity;

        *data++ = Math::Lerp(segment.start.x, segment.end.x, 1.0f - beginEndSizeCoe + beginEndOffset) + segment.normal.x * width * segment.intensity;
        *data++ = Math::Lerp(segment.start.y, segment.end.y, 1.0f - beginEndSizeCoe + beginEndOffset) + segment.normal.y * width * segment.intensity;
        *data++ = 0.75f;
        *data++ = 0;
        *data++ = 1;
        *data++ = 1;
        *data++ = 1;
        *data++ = segment.intensity;

        *data++ = Math::Lerp(segment.start.x, segment.end.x, 1.0f - beginEndSizeCoe + beginEndOffset) - segment.normal.x * width * segment.intensity;
        *data++ = Math::Lerp(segment.start.y, segment.end.y, 1.0f - beginEndSizeCoe + beginEndOffset) - segment.normal.y * width * segment.intensity;
        *data++ = 0.75f;
        *data++ = 1;
        *data++ = 1;
        *data++ = 1;
        *data++ = 1;
        *data++ = segment.intensity;

        // end
        *data++ = Math::Lerp(segment.start.x, segment.end.x, 1.0f - beginEndSizeCoe + beginEndOffset) + segment.normal.x * width * segment.intensity;
        *data++ = Math::Lerp(segment.start.y, segment.end.y, 1.0f - beginEndSizeCoe + beginEndOffset) + segment.normal.y * width * segment.intensity;
        *data++ = 0.75f;
        *data++ = 0;
        *data++ = 1;
        *data++ = 1;
        *data++ = 1;
        *data++ = segment.intensity;

        *data++ = Math::Lerp(segment.start.x, segment.end.x, 1.0f - beginEndSizeCoe + beginEndOffset) - segment.normal.x * width * segment.intensity;
        *data++ = Math::Lerp(segment.start.y, segment.end.y, 1.0f - beginEndSizeCoe + beginEndOffset) - segment.normal.y * width * segment.intensity;
        *data++ = 0.75f;
        *data++ = 1;
        *data++ = 1;
        *data++ = 1;
        *data++ = 1;
        *data++ = segment.intensity;

        *data++ = Math::Lerp(segment.start.x, segment.end.x, 1.0f + beginEndOffset) + segment.normal.x * width * segment.intensity;
        *data++ = Math::Lerp(segment.start.y, segment.end.y, 1.0f + beginEndOffset) + segment.normal.y * width * segment.intensity;
        *data++ = 1;
        *data++ = 0;
        *data++ = 1;
        *data++ = 1;
        *data++ = 1;
        *data++ = segment.intensity;

        *data++ = Math::Lerp(segment.start.x, segment.end.x, 1.0f + beginEndOffset) - segment.normal.x * width * segment.intensity;
        *data++ = Math::Lerp(segment.start.y, segment.end.y, 1.0f + beginEndOffset) - segment.normal.y * width * segment.intensity;
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
EGE::List<Segment> LightningEffect::generateSegments(const Vector2f& start, const Vector2f end, s32 steps, bool allowOffshots)
{
  List<Segment> segments;

  Segment segment;
  segment.start     = start;
  segment.end       = end;
  segment.normal    = (end - start).perpendicular().normalized();
  segment.intensity = 1.0f;
  segments.push_back(segment);

  float32 offset = 60;
  for (s32 step = 0; step < steps; ++step)
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
      segments.insert(it, segment);

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

          segment.end = segment.end * dirLength * 0.7f + midPoint;
          segment.normal    = (segment.end - segment.start).perpendicular().normalized();
          segment.intensity = oldSegment.intensity * 0.5f;
          segments.push_front(segment);
        }
      }
    }

    // update offset magnitude
    offset *= 0.5f;
  }

  return segments;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
