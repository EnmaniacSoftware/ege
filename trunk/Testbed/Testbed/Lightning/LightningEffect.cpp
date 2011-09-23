#include "LightningEffect.h"
#include <EGEMath.h>
#include <EGEDebug.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
LightningEffect::LightningEffect(Application* app) : SceneNodeObject("lightning-effect"), m_app(app)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
LightningEffect::~LightningEffect()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Initializes object. */
//bool LightningEffect::initialize(s32 width, s32 height, const Vector2i& gridSize, PCamera camera)
//{
//  m_width  = width;
//  m_height = height;
//
//  m_gridSize = gridSize;
//  
//  m_defaultTextureCoords.resize(m_gridSize.x * m_gridSize.y);
//
//  // create render texture
//  m_texture = Texture2D::CreateRenderTexture(m_app, "LightningEffect::rttTex", width, height, EGEImage::RGB_888);
//
//  // setup render texture
//  PRenderTarget renderTarget = m_texture->renderTarget();
//  if (NULL == renderTarget)
//  {
//    // error!
//    return false;
//  }
//
//  // add viewport for render target
//  if (NULL == renderTarget->addViewport("main", camera))
//  {
//    // error!
//    return false;
//  }
//
//  // create render data
//  m_renderData = ege_new RenderComponent(m_app, name(), EGEGraphics::RP_MAIN);
//  if ((NULL == m_renderData) || !m_renderData->vertexBuffer()->setSemantics(EGEVertexBuffer::ST_V3_T2) ||
//      !m_renderData->indexBuffer()->create(IndexBuffer::SIZE_16BIT, 6 * (m_gridSize.x - 1) * (m_gridSize.y - 1)))
//  {
//    // error!
//    return false;
//  }
//
//  // setup index data
//  u16* indexData = reinterpret_cast<u16*>(m_renderData->indexBuffer()->lock(0, 6 * (m_gridSize.x - 1) * (m_gridSize.y - 1)));
//  for (s32 i = 0; i < m_gridSize.x - 1; ++i)
//  {
//    for (s32 j = 0; j < m_gridSize.y - 1; ++j)
//    {
//      *indexData++ = static_cast<u16>(i * m_gridSize.y + j);
//      *indexData++ = static_cast<u16>(i * m_gridSize.y + j + 1);
//      *indexData++ = static_cast<u16>(i * m_gridSize.y + j + 1 + m_gridSize.y);
//      *indexData++ = static_cast<u16>(i * m_gridSize.y + j);
//      *indexData++ = static_cast<u16>(i * m_gridSize.y + j + 1 + m_gridSize.y);
//      *indexData++ = static_cast<u16>(i * m_gridSize.y + j + m_gridSize.y);
//    }
//  }
//  m_renderData->indexBuffer()->unlock();
//
//  // create material with render texture for render texture object
//  PMaterial material = ege_new Material(app());
//  //material->setDiffuseColor(Color::GREEN);
//  material->addTexture(m_texture);
//  m_renderData->setMaterial(material);
//
//  // initialize ripple data
//  for (s32 i = 0; i < RIPPLE_COUNT; ++i)
//  {
//    m_times[i]    = static_cast<s32>(Math::Sqrt(width * width + height * height)) + RIPPLE_LENGTH;
//    m_maxTimes[i] = 0;
//    m_centers[i].set(0, 0);
//  }
//
//  // setup render data
//  float32* data = reinterpret_cast<float32*>(m_renderData->vertexBuffer()->lock(0, m_gridSize.x * m_gridSize.y));
//  for (s32 i = 0; i < m_gridSize.x; i++)
//  {
//    for (s32 j = 0; j < m_gridSize.y; j++)
//    {
//      // position (constant all the time)
//      *data++ = i / (m_gridSize.x - 1.0f) * width;
//      *data++ = j / (m_gridSize.y - 1.0f) * height;
//      *data++ = 0.0f;
//
//      // default texture coords
//      m_defaultTextureCoords[i * m_gridSize.y + j].set(i / (m_gridSize.x - 1.0f), j / (m_gridSize.y - 1.0f));
//
//      // current texture coords
//      *data++ = m_defaultTextureCoords[i * m_gridSize.y + j].x;
//      *data++ = m_defaultTextureCoords[i * m_gridSize.y + j].y;
//    }
//  }
//  m_renderData->vertexBuffer()->unlock();
//
//  precalculate();
//
//  return true;
//}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! SceneNodeObject override. Adds object render data for rendering with given renderer. */
bool LightningEffect::addForRendering(Renderer* renderer)
{
  return renderer->addForRendering(parentNode()->worldMatrix(), m_renderData);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates effect. */
void LightningEffect::update(const Time& time)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Pointer event receiver. */
void LightningEffect::pointerEvent(PPointerData data)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
