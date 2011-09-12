#include "RippleEffect.h"
extern "C"
{
#include "ripple.h"
}
#include <EGEMath.h>
#include <EGEDebug.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RippleEffect::RippleEffect(Application* app) : SceneNodeObject("ripple-effect"), m_app(app)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RippleEffect::~RippleEffect()
{
  m_texture = NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Initializes object. */
bool RippleEffect::initialize(s32 width, s32 height, PCamera camera)
{
  m_maxRipple = static_cast<s32>(Math::Sqrt(width * width + height * height));
  
  // create render texture
  m_texture = Texture2D::CreateRenderTexture(m_app, "RippleEffect::rttTex", width, height, EGEImage::RGB_888);

  // setup render texture
  PRenderTarget renderTarget = m_texture->renderTarget();
  if (NULL == renderTarget)
  {
    // error!
    return false;
  }

  // add viewport for render target
  if (NULL == renderTarget->addViewport("main", camera))
  {
    // error!
    return false;
  }

  // create render data
  m_renderData = ege_new RenderComponent(m_app, name(), EGEGraphics::RP_MAIN);
  if ((NULL == m_renderData) || !m_renderData->vertexBuffer()->setSemantics(EGEVertexBuffer::ST_V3_T2) ||
      !m_renderData->indexBuffer()->create(IndexBuffer::SIZE_16BIT, 6 * GRID_SIZE_X * GRID_SIZE_Y))
  {
    // error!
    return false;
  }

  u16* indexData = reinterpret_cast<u16*>(m_renderData->indexBuffer()->lock(0, 6 * GRID_SIZE_X * GRID_SIZE_Y));
  for (s32 i = 0; i < GRID_SIZE_X; ++i)
  {
    for (s32 j = 0; j < GRID_SIZE_Y; ++j)
    {
      *indexData++ = i * GRID_SIZE_X + j;
      *indexData++ = i * GRID_SIZE_X + 1 + j;
      *indexData++ = (i + 1) * GRID_SIZE_X + 1 + j;
      *indexData++ = i * GRID_SIZE_X + j;
      *indexData++ = (i + 1) * GRID_SIZE_X + 1 + j;
      *indexData++ = (i + 1) * GRID_SIZE_X + j;
    }
  }

  m_renderData->indexBuffer()->unlock();

  // setup render data
  float32* data = reinterpret_cast<float32*>(m_renderData->vertexBuffer()->lock(0, GRID_SIZE_X * GRID_SIZE_Y));

  Vector2f vertexPos(0, 0);
  Vector2f vertexPosIncrement(width / static_cast<float32>(GRID_SIZE_X), height / static_cast<float32>(GRID_SIZE_Y));
  Vector2f uvPos(0, 0);
  Vector2f uvPosIncrement(1.0f / GRID_SIZE_X, 1.0f / GRID_SIZE_Y);

  // go thru all grid cells
 /* for (s32 i = 0; i < GRID_SIZE_X; ++i)
  {
    for (s32 j = 0; j < GRID_SIZE_Y; ++j)
    {
      // Cell quad looks like follows:
      //
      //   (0,3)  (5)
      //    *------*
      //    |\     |
      //    | \Tri2|
      //    |  \   |
      //    |   \  |
      //    |    \ |
      //    |Tri1 \|
      //    |      |
      //    *------*
      //   (1)   (2,4)

      // 0
      *data++ = vertexPos.x;
      *data++ = vertexPos.y;
      *data++ = 0.0f;
      *data++ = uvPos.x;
      *data++ = uvPos.y;

      // 1
      *data++ = vertexPos.x;
      *data++ = vertexPos.y + vertexPosIncrement.y;
      *data++ = 0.0f;
      *data++ = uvPos.x;
      *data++ = uvPos.y + uvPosIncrement.y;

      // 2
      *data++ = vertexPos.x + vertexPosIncrement.x;
      *data++ = vertexPos.y + vertexPosIncrement.y;
      *data++ = 0.0f;
      *data++ = uvPos.x + uvPosIncrement.x;
      *data++ = uvPos.y + uvPosIncrement.y;

      // 3
      *data++ = vertexPos.x;
      *data++ = vertexPos.y;
      *data++ = 0.0f;
      *data++ = uvPos.x;
      *data++ = uvPos.y;

      // 4
      *data++ = vertexPos.x + vertexPosIncrement.x;
      *data++ = vertexPos.y + vertexPosIncrement.y;
      *data++ = 0.0f;
      *data++ = uvPos.x + uvPosIncrement.x;
      *data++ = uvPos.y + uvPosIncrement.y;
    
      // 5
      *data++ = vertexPos.x + vertexPosIncrement.x;
      *data++ = vertexPos.y;
      *data++ = 0.0f;
      *data++ = uvPos.x + uvPosIncrement.x;
      *data++ = uvPos.y;

      // go to next cell in Y direction
      vertexPos.y += vertexPosIncrement.y;
      uvPos.y += uvPosIncrement.y;
    }

    // reset Y direction data
    vertexPos.y = 0;
    uvPos.y = 0;

    // go to next cell in X direction
    vertexPos.x += vertexPosIncrement.x;
    uvPos.x += uvPosIncrement.x;
  }*/

  m_renderData->vertexBuffer()->unlock();
 
  // create material with render texture for render texture object
  PMaterial material = ege_new Material(app());
  material->setDiffuseColor(Color::GREEN);
  material->addTexture(m_texture);
  m_renderData->setMaterial(material);


  ripple_init();

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! SceneNodeObject override. Adds object render data for rendering with given renderer. */
bool RippleEffect::addForRendering(Renderer* renderer)
{
  // setup render data
  float32* data = reinterpret_cast<float32*>(m_renderData->vertexBuffer()->lock(0, (GRID_SIZE_X + 1) * (GRID_SIZE_Y + 1)));
  
#if 1
  // go thru all grid cells
  for (s32 i = 0; i < GRID_SIZE_X; ++i)
  {
    for (s32 j = 0; j < GRID_SIZE_Y; ++j)
    {
      // Cell quad looks like follows:
      //
      //   (0,3)  (5)
      //    *------*
      //    |\     |
      //    | \Tri2|
      //    |  \   |
      //    |   \  |
      //    |    \ |
      //    |Tri1 \|
      //    |      |
      //    *------*
      //   (1)   (2,4)

      // 0
      *data++ = ripple_vertex[i][j].x[0];
      *data++ = ripple_vertex[i][j].x[1];
      *data++ = 0.0f;
      *data++ = ripple_vertex[i][j].t[0];
      *data++ = ripple_vertex[i][j].t[1];
      /*
      // 1
      *data++ = ripple_vertex[i][j + 1].x[0];
      *data++ = ripple_vertex[i][j + 1].x[1];
      *data++ = 0.0f;
      *data++ = ripple_vertex[i][j + 1].t[0];
      *data++ = ripple_vertex[i][j + 1].t[1];

      // 2
      *data++ = ripple_vertex[i + 1][j + 1].x[0];
      *data++ = ripple_vertex[i + 1][j + 1].x[1];
      *data++ = 0.0f;
      *data++ = ripple_vertex[i + 1][j + 1].t[0];
      *data++ = ripple_vertex[i + 1][j + 1].t[1];

      // 3
      *data++ = ripple_vertex[i][j].x[0];
      *data++ = ripple_vertex[i][j].x[1];
      *data++ = 0.0f;
      *data++ = ripple_vertex[i][j].t[0];
      *data++ = ripple_vertex[i][j].t[1];

      // 4
      *data++ = ripple_vertex[i + 1][j + 1].x[0];
      *data++ = ripple_vertex[i + 1][j + 1].x[1];
      *data++ = 0.0f;
      *data++ = ripple_vertex[i + 1][j + 1].t[0];
      *data++ = ripple_vertex[i + 1][j + 1].t[1];
    
      // 5
      *data++ = ripple_vertex[i + 1][j].x[0];
      *data++ = ripple_vertex[i + 1][j].x[1];
      *data++ = 0.0f;
      *data++ = ripple_vertex[i + 1][j].t[0];
      *data++ = ripple_vertex[i + 1][j].t[1];*/
    }
  }
#endif
  m_renderData->vertexBuffer()->unlock();

  return renderer->addForRendering(parentNode()->worldMatrix(), m_renderData);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates effect. */
void RippleEffect::update(const Time& time)
{
  ripple_dynamics(0, 0);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Pointer event receiver. */
void RippleEffect::pointerEvent(PPointerData data)
{
  if (EGEInput::ACTION_BUTTON_UP == data->action())
  {
    ripple_click(data->x(), data->y(), 1);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
