#include "LightningEffect.h"
#include <EGEMath.h>
#include <EGEDebug.h>
#include <EGEResources.h>
#include <EGEOverlay.h>
#include <EGEApplication.h>
//#include <gl/GL.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
LightningEffect::LightningEffect(Application* app) : SceneNodeObject("lightning-effect"), 
                                                     m_app(app)
{
    // setup vertex declaration
    VertexDeclaration vertexDeclaration;
    if ( ! vertexDeclaration.addElement(NVertexBuffer::VES_POSITION_XYZ))
    {
      // error!
      EGE_DELETE(object);
      return NULL;
    }

    if (uv && ! vertexDeclaration.addElement(NVertexBuffer::VES_TEXTURE_UV))
    {
      // error!
      EGE_DELETE(object);
      return NULL;
    }

    if (color && ! vertexDeclaration.addElement(NVertexBuffer::VES_COLOR_RGBA))
    {
      // error!
      EGE_DELETE(object);
      return NULL;
    }

  m_renderData = ege_new RenderComponent(app, "lightning-effect-lines", EGEGraphics::RP_MAIN, EGEGraphics::RPT_LINES);
  m_renderDataQuad = ege_new RenderComponent(app, "lightning-effect-quads-1", EGEGraphics::RP_MAIN + 1, EGEGraphics::RPT_TRIANGLES);
  m_renderDataQuad2 = ege_new RenderComponent(app, "lightning-effect-quads-2", EGEGraphics::RP_MAIN + 1, EGEGraphics::RPT_TRIANGLES);
  if (m_renderData)
  {
    m_renderData->vertexBuffer()->setSemantics(EGEVertexBuffer::ST_V2_C4);
    m_renderDataQuad->vertexBuffer()->setSemantics(EGEVertexBuffer::ST_V2_T2_C4);
    m_renderDataQuad2->vertexBuffer()->setSemantics(EGEVertexBuffer::ST_V2_T2_C4);
    m_renderData->setLineWidth(2.0f);

    PMaterial material = ege_new Material(app);
    RenderPass* pass = material->addPass(NULL);
    m_renderData->setMaterial(material);
    material->setSrcBlendFactor(EGEGraphics::BF_SRC_ALPHA);
    material->setDstBlendFactor(EGEGraphics::BF_ONE_MINUS_SRC_ALPHA);

    PResourceMaterial resource = app->resourceManager()->resource(RESOURCE_NAME_MATERIAL, "beam");
    m_renderDataQuad->setMaterial(resource->createInstance());
    m_renderDataQuad2->setMaterial(resource->createInstance());
  }

  m_renderDataQuad->indexBuffer()->setIndexSize(EGEIndexBuffer::IS_16BIT);
  m_renderDataQuad2->indexBuffer()->setIndexSize(EGEIndexBuffer::IS_16BIT);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
LightningEffect::~LightningEffect()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! SceneNodeObject override. Adds object render data for rendering with given renderer. */
bool LightningEffect::addForRendering(IRenderer* renderer, const Matrix4f& transform)
{
  if (m_fadeTime.seconds() < 1.0f)
  {
    //renderer->addForRendering(m_renderData, parentNode()->worldMatrix().multiply(transform));
    //renderer->addForRendering(m_renderDataQuad, parentNode()->worldMatrix().multiply(transform));
    //renderer->addForRendering(m_renderDataQuad2, parentNode()->worldMatrix().multiply(transform));
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates effect. */
void LightningEffect::update(const Time& time)
{
  m_fadeTime += time * 2.0f;
  if (m_fadeTime.seconds() > 0.5f)
  {
    m_fadeTime = 0.5f;
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
  m_renderData->vertexBuffer()->unlock(data - 1);

  data = (float32*) m_renderDataQuad->vertexBuffer()->lock(0, m_segments.size() * 8);

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
  }

  m_renderDataQuad->vertexBuffer()->unlock(data - 1);

  data = (float32*) m_renderDataQuad2->vertexBuffer()->lock(0, m_segments2.size() * 8);

  for (List<Segment2*>::const_iterator it = m_segments2.begin(); it != m_segments2.end(); ++it)
  {
    const Segment2 segment= **it;

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

  m_renderDataQuad2->vertexBuffer()->unlock(data - 1);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Pointer event receiver. */
void LightningEffect::pointerEvent(PPointerData data)
{
  bool color = false;

  if (ACTION_BUTTON_DOWN == data->action())
  {
    m_fadeTime = 0.0f;

    m_segments = generateSegments(Vector2f(100, 50), Vector2f(600, 250), 5, true);
    m_segments2 = generateSegments2(Vector2f(100, 350), Vector2f(600, 500), 5, true);

    float32* data = (float32*) m_renderData->vertexBuffer()->lock(0, m_segments.size() * 2);
    for (List<Segment>::const_iterator it = m_segments.begin(); it != m_segments.end(); ++it)
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

    m_renderData->vertexBuffer()->unlock(data - 1);

    {
      float32* data = (float32*) m_renderDataQuad->vertexBuffer()->lock(0, m_segments.size() * 8);
      s16* index = (s16*) m_renderDataQuad->indexBuffer()->lock(0, m_segments.size() * 6 * 3);

      const float32 width = 1.0f;
      const float32 beginEndSizeCoe = 0.05f;
      const float32 beginEndOffset = 0.015f;

      int i = 0;
      for (List<Segment>::const_iterator it = m_segments.begin(); it != m_segments.end(); ++it, ++i)
      {
        const Segment& segment= *it;

        // begin
        *index++ = i * 8 + 0;
        *index++ = i * 8 + 1;
        *index++ = i * 8 + 2;
        *index++ = i * 8 + 1;
        *index++ = i * 8 + 2;
        *index++ = i * 8 + 3;

        // middle
        *index++ = i * 8 + 2;
        *index++ = i * 8 + 3;
        *index++ = i * 8 + 4;
        *index++ = i * 8 + 3;
        *index++ = i * 8 + 4;
        *index++ = i * 8 + 5;

        // end
        *index++ = i * 8 + 4;
        *index++ = i * 8 + 5;
        *index++ = i * 8 + 6;
        *index++ = i * 8 + 5;
        *index++ = i * 8 + 6;
        *index++ = i * 8 + 7;

        // begin
        *data++ = Math::Lerp(segment.start.x, segment.end.x, -beginEndOffset) + segment.startNormal.x * width * segment.intensity;
        *data++ = Math::Lerp(segment.start.y, segment.end.y, -beginEndOffset) + segment.startNormal.y * width * segment.intensity;
        *data++ = segment._begin ? 0.0f : 0.25f;
        *data++ = 0;
        *data++ = 1;
        *data++ = (color) ? 0.0f : 1.0f;
        *data++ = (color) ? 0.0f : 1.0f;
        *data++ = segment.intensity;

        *data++ = Math::Lerp(segment.start.x, segment.end.x, -beginEndOffset) - segment.startNormal.x * width * segment.intensity;
        *data++ = Math::Lerp(segment.start.y, segment.end.y, -beginEndOffset) - segment.startNormal.y * width * segment.intensity;
        *data++ = segment._begin ? 0.0f : 0.25f;
        *data++ = 1;
        *data++ = 1;
        *data++ = (color) ? 0.0f : 1.0f;
        *data++ = (color) ? 0.0f : 1.0f;
        *data++ = segment.intensity;

        // end of begin begining of middle
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
        *data++ = Math::Lerp(segment.start.x, segment.end.x, 1.0f + beginEndOffset) + segment.endNormal.x * width * segment.intensity;
        *data++ = Math::Lerp(segment.start.y, segment.end.y, 1.0f + beginEndOffset) + segment.endNormal.y * width * segment.intensity;
        *data++ = segment._end ? 1.0f : 0.75f;
        *data++ = 0;
        *data++ = (color) ? 0.0f : 1.0f;
        *data++ = 1;
        *data++ = (color) ? 0.0f : 1.0f;
        *data++ = segment.intensity;

        *data++ = Math::Lerp(segment.start.x, segment.end.x, 1.0f + beginEndOffset) - segment.endNormal.x * width * segment.intensity;
        *data++ = Math::Lerp(segment.start.y, segment.end.y, 1.0f + beginEndOffset) - segment.endNormal.y * width * segment.intensity;
        *data++ = segment._end ? 1.0f : 0.75f;
        *data++ = 1;
        *data++ = (color) ? 0.0f : 1.0f;
        *data++ = 1;
        *data++ = (color) ? 0.0f : 1.0f;
        *data++ = segment.intensity;
      }

      m_renderDataQuad->vertexBuffer()->unlock(data - 1);
      m_renderDataQuad->indexBuffer()->unlock(index - 1);
    }

    {
      float32* data = (float32*) m_renderDataQuad2->vertexBuffer()->lock(0, m_segments2.size() * 8);
      s16* index = (s16*) m_renderDataQuad2->indexBuffer()->lock(0, m_segments2.size() * 6 * 3);

      const float32 width = 1.0f;
      const float32 beginEndSizeCoe = 0.2f;

      Vector2f normal;

      int i = 0;
      for (List<Segment2*>::const_iterator it = m_segments2.begin(); it != m_segments2.end(); ++it, ++i)
      {
        const Segment2 segment = **it;

        // begin
        *index++ = i * 8 + 0;
        *index++ = i * 8 + 1;
        *index++ = i * 8 + 2;
        *index++ = i * 8 + 1;
        *index++ = i * 8 + 2;
        *index++ = i * 8 + 3;

        // middle
        *index++ = i * 8 + 2;
        *index++ = i * 8 + 3;
        *index++ = i * 8 + 4;
        *index++ = i * 8 + 3;
        *index++ = i * 8 + 4;
        *index++ = i * 8 + 5;

        // end
        *index++ = i * 8 + 4;
        *index++ = i * 8 + 5;
        *index++ = i * 8 + 6;
        *index++ = i * 8 + 5;
        *index++ = i * 8 + 6;
        *index++ = i * 8 + 7;

        bool isOffshot = segment.prev && (segment.prev->offshot == *it);

        // begin
        if (segment.prev && !isOffshot)
        {
          normal = (segment.prev->normal + segment.normal).normalized();
        }
        else
        {
          normal = segment.normal;
        }

        *data++ = Math::Lerp(segment.start.x, segment.end.x, 0) + normal.x * width * segment.intensity;
        *data++ = Math::Lerp(segment.start.y, segment.end.y, 0) + normal.y * width * segment.intensity;
        *data++ = segment._begin ? 0 : 0.25f;
        *data++ = 0;
        *data++ = 1;
        *data++ = (color) ? 0.0f : 1.0f;
        *data++ = (color) ? 0.0f : 1.0f;
        *data++ = segment.intensity;

        *data++ = Math::Lerp(segment.start.x, segment.end.x, 0) - normal.x * width * segment.intensity;
        *data++ = Math::Lerp(segment.start.y, segment.end.y, 0) - normal.y * width * segment.intensity;
        *data++ = segment._begin ? 0.0f : 0.25f;
        *data++ = 1;
        *data++ = 1;
        *data++ = (color) ? 0.0f : 1.0f;
        *data++ = (color) ? 0.0f : 1.0f;
        *data++ = segment.intensity;

        // end of begin begining of middle
        *data++ = Math::Lerp(segment.start.x, segment.end.x, beginEndSizeCoe) + segment.normal.x * width * segment.intensity;
        *data++ = Math::Lerp(segment.start.y, segment.end.y, beginEndSizeCoe) + segment.normal.y * width * segment.intensity;
        *data++ = 0.25f;
        *data++ = 0;
        *data++ = 1;
        *data++ = 1;
        *data++ = 1;
        *data++ = segment.intensity;

        *data++ = Math::Lerp(segment.start.x, segment.end.x, beginEndSizeCoe) - segment.normal.x * width * segment.intensity;
        *data++ = Math::Lerp(segment.start.y, segment.end.y, beginEndSizeCoe) - segment.normal.y * width * segment.intensity;
        *data++ = 0.25f;
        *data++ = 1;
        *data++ = 1;
        *data++ = 1;
        *data++ = 1;
        *data++ = segment.intensity;

        *data++ = Math::Lerp(segment.start.x, segment.end.x, 1.0f - beginEndSizeCoe) + segment.normal.x * width * segment.intensity;
        *data++ = Math::Lerp(segment.start.y, segment.end.y, 1.0f - beginEndSizeCoe) + segment.normal.y * width * segment.intensity;
        *data++ = 0.75f;
        *data++ = 0;
        *data++ = 1;
        *data++ = 1;
        *data++ = 1;
        *data++ = segment.intensity;

        *data++ = Math::Lerp(segment.start.x, segment.end.x, 1.0f - beginEndSizeCoe) - segment.normal.x * width * segment.intensity;
        *data++ = Math::Lerp(segment.start.y, segment.end.y, 1.0f - beginEndSizeCoe) - segment.normal.y * width * segment.intensity;
        *data++ = 0.75f;
        *data++ = 1;
        *data++ = 1;
        *data++ = 1;
        *data++ = 1;
        *data++ = segment.intensity;

        if (segment.next)
        {
          normal = (segment.next->normal + segment.normal).normalized();
        }
        else
        {
          normal = segment.normal;
        }

        // end
        *data++ = Math::Lerp(segment.start.x, segment.end.x, 1.0f) + normal.x * width * segment.intensity;
        *data++ = Math::Lerp(segment.start.y, segment.end.y, 1.0f) + normal.y * width * segment.intensity;
        *data++ = segment._end ? 1.0f : 0.75f;
        *data++ = 0;
        *data++ = (color) ? 0.0f : 1.0f;
        *data++ = 1;
        *data++ = (color) ? 0.0f : 1.0f;
        *data++ = segment.intensity;

        *data++ = Math::Lerp(segment.start.x, segment.end.x, 1.0f) - normal.x * width * segment.intensity;
        *data++ = Math::Lerp(segment.start.y, segment.end.y, 1.0f) - normal.y * width * segment.intensity;
        *data++ = segment._end ? 1.0f : 0.75f;
        *data++ = 1;
        *data++ = (color) ? 0.0f : 1.0f;
        *data++ = 1;
        *data++ = (color) ? 0.0f : 1.0f;
        *data++ = segment.intensity;
      }

      m_renderDataQuad2->vertexBuffer()->unlock(data - 1);
      m_renderDataQuad2->indexBuffer()->unlock(index - 1);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Generates segments. */
EGE::List<Segment> LightningEffect::generateSegments(const Vector2f& start, const Vector2f end, s32 steps, bool allowOffshots)
{
  List<Segment> segments;

  static int a = 0;

  Segment segment;
  segment.start         = start;
  segment.end           = end;
  segment._begin        = true;
  segment._end          = true;
  segment.normal        = (end - start).perpendicular().normalized();
  segment.startNormal   = segment.normal;
  segment.endNormal     = segment.normal;
  segment.intensity     = 1.0f;
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
      if (/*m_random()*/ a & 0x1)
      {
        offsetVector *= -1.0f;
      }
        a++;

      // move mid point along the perpendicular vectors
      midPoint += offsetVector * offset;

      // update current segment to second subsegment
      (*it).start = midPoint;
      (*it).end  = oldSegment.end;
      (*it).normal = (oldSegment.end - midPoint).perpendicular().normalized();
      (*it).endNormal = (*it).normal;
      (*it)._begin = false;

      // create new segment for first subsegment
      segment.start     = oldSegment.start;
      segment.end       = midPoint;
      segment.normal    = (segment.end - segment.start).perpendicular().normalized();
      segment.startNormal    = segment.normal;
      segment.intensity = oldSegment.intensity;
      segment._begin    = oldSegment._begin;
      segment._end      = false;

      (*it).startNormal = ((*it).normal + segment.normal).normalized();
      segment.endNormal = (*it).startNormal;

      segments.insert(it, segment);

      // check if offshot should be generated
      if (allowOffshots)
      {
        if ((Math::Random()() % 100) < oldSegment.intensity * 50)
        {
          segment.start = midPoint;

          Vector2f direction = midPoint - oldSegment.start;
          float32 dirLength = direction.length();
          direction.normalize();

          Angle angle = Angle::FromDegrees(Math::Random()(5, 10));

          float32 cos = Math::Cos(angle.radians());
          float32 sin = Math::Sin(angle.radians());

          segment.end.x = direction.x * cos - direction.y * sin;
          segment.end.y = direction.x * sin + direction.y * cos;

          segment.end     = segment.end * dirLength * 0.7f + midPoint;
          segment.normal  = (segment.end - segment.start).perpendicular().normalized();
          segment._begin  = false;
          segment._end    = true;
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
/*! Generates segments. */
List<Segment2*> LightningEffect::generateSegments2(const EGE::Vector2f& start, const EGE::Vector2f end, EGE::s32 steps, bool allowOffshots)
{
  List<Segment2*> segments;

  static int a = 0;

  Segment2* segment = new Segment2;
  segment->start         = start;
  segment->end           = end;
  segment->_begin        = true;
  segment->_end          = true;
  segment->normal        = (end - start).perpendicular().normalized();
  segment->intensity     = 1.0f;
  segment->prev          = NULL;
  segment->next          = NULL;
  segment->offshot       = NULL;
  segments.push_back(segment);

  float32 offset = 60;
  for (s32 step = 0; step < steps; ++step)
  {
    // go thru all segments
    s32 count = segments.size();
    for (List<Segment2*>::iterator it = segments.begin(); count > 0; ++it, --count)
    {
      Segment2 oldSegment = **it;

      // determine segment vector
      Vector2f segmentVector = oldSegment.end - oldSegment.start;

      // get point in the middle of segment
      Vector2f midPoint(oldSegment.start.x + segmentVector.x * 0.5f, oldSegment.start.y + segmentVector.y * 0.5f);

      // calulate normalized vector perpendicular to current segment
      Vector2f offsetVector = segment->normal;
      if (/*m_random()*/ a & 0x1)
      {
        offsetVector *= -1.0f;
      }
      a++;

      // move mid point along the perpendicular vectors
      midPoint += offsetVector * offset;

      // update current segment to second subsegment
      (*it)->start = midPoint;
      (*it)->end  = oldSegment.end;
      (*it)->normal = (oldSegment.end - midPoint).perpendicular().normalized();
      (*it)->_begin = false;

      // create new segment for first subsegment
      Segment2* newSegment = new Segment2;
      newSegment->start     = oldSegment.start;
      newSegment->end       = midPoint;
      newSegment->normal    = (newSegment->end - newSegment->start).perpendicular().normalized();
      newSegment->intensity = oldSegment.intensity;
      newSegment->_begin    = oldSegment._begin;
      newSegment->_end      = false;
      newSegment->offshot   = NULL;

      (*it)->prev = newSegment;
      newSegment->next = *it;
      newSegment->prev = oldSegment.prev;

      if (oldSegment.prev)
      {
        oldSegment.prev->next = newSegment;
      }

      segments.insert(it, newSegment);

      // check if offshot should be generated
      if (allowOffshots)
      {
        if (((Math::Random()() % 100) < oldSegment.intensity * 50) && (NULL == (*it)->prev->offshot))
        {
          Segment2* newSegment = new Segment2;

          newSegment->start = midPoint;

          Vector2f direction = midPoint - (*it)->prev->start;
          float32 dirLength = direction.length();
          direction.normalize();

          Angle angle = Angle::FromDegrees(Math::Random()(10, 15));

          float32 cos = Math::Cos(angle.radians());
          float32 sin = Math::Sin(angle.radians());

          newSegment->end.x = direction.x * cos - direction.y * sin;
          newSegment->end.y = direction.x * sin + direction.y * cos;

          newSegment->end     = newSegment->end * dirLength * 0.7f + midPoint;
          newSegment->normal  = (newSegment->end - newSegment->start).perpendicular().normalized();
          newSegment->_begin  = false;
          newSegment->_end    = true;
          newSegment->prev    = (*it)->prev;
          newSegment->next    = NULL;
          newSegment->offshot = NULL;

          (*it)->prev->offshot = newSegment;

          newSegment->intensity = oldSegment.intensity * 0.5f;
          segments.push_back(newSegment);
        }
      }
    }

    // update offset magnitude
    offset *= 0.5f;
  }

  return segments;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
