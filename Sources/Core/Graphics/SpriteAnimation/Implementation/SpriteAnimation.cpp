#include "Core/Graphics/SpriteAnimation/Interface/SpriteAnimation.h"
#include "Core/Resource/ResourceSpritesheet.h"
#include "Core/Resource/ResourceTextureImage.h"
#include "EGESequencer.h"
#include "EGEMaterial.h"
#include "EGEPhysics.h"
#include "EGEGraphics.h"
#include "EGERenderComponent.h"
#include "EGERenderer.h"
#include "EGEDebug.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const char* KSpriteAnimationDebugName = "EGESpriteAnimation";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SpriteAnimation::SpriteAnimation(Application* app, const String& name) : KeyFrameAnimation(app, name) 
                                                                       , m_baseAlignment(ALIGN_TOP_LEFT)
                                                                       , m_alpha(1.0f)
                                                                       , m_renderDataNeedsUpdate(false)
                                                                       , m_transform(Matrix4f::IDENTITY)
                                                                       , m_displaySize(50, 50)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SpriteAnimation::~SpriteAnimation()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SpriteAnimation::construct()
{
  // create render data
  m_renderData = RenderObjectFactory::CreateQuadXY(app(), name(), Vector4f::ZERO, Vector2f::ONE, ALIGN_TOP_LEFT, false, false, RenderObjectFactory::VS_V2_T2_C4, 
                                                   EGEGraphics::RP_MAIN, EGEGraphics::RPT_TRIANGLES, NVertexBuffer::UT_DYNAMIC_WRITE_DONT_CARE);
  if (NULL == m_renderData)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  // create local material
  PMaterial material = ege_new Material(app());
  if (NULL == material)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  PRenderPass pass = material->addPass(NULL);
  if (NULL == pass)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  material->setSrcBlendFactor(EGEGraphics::BF_SRC_ALPHA);
  material->setDstBlendFactor(EGEGraphics::BF_ONE_MINUS_SRC_ALPHA);

  // associate material with render data
  m_renderData->setMaterial(material);

  // create physics data
  m_physicsData = ege_new PhysicsComponent(app(), name());
  if (NULL == m_physicsData)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  ege_connect(m_physicsData, transformationChanged, this, SpriteAnimation::onTransformationChanged);

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void SpriteAnimation::setFrameData(const DynamicArray<SpriteAnimationFrameData>& data)
{
  m_frameData.copy(data);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void SpriteAnimation::setTexture(const PTextureImage& texture)
{
  EGE_ASSERT(NULL != m_renderData);
  EGE_ASSERT(NULL != m_renderData->material());
  EGE_ASSERT(NULL != m_renderData->material()->pass(0));

  EGEResult result = EGE_ERROR_ALREADY_EXISTS;

  // check if any texture present in render pass
  PRenderPass pass = m_renderData->material()->pass(0);
  if (0 < pass->textureCount())
  {
    // replace if different texture
    if (pass->texture(0) != texture)
    {
      result = pass->setTexture(0, texture);
    }
  }
  else
  {
    // add texture
    result = pass->addTexture(texture);
  }

  if (EGE_SUCCESS == result)
  {
    // make sure vertex data gets updated
    m_renderDataNeedsUpdate = true;
  }

  EGE_ASSERT((EGE_ERROR_ALREADY_EXISTS == result) || (EGE_SUCCESS == result));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SpriteAnimation::addForRendering(IRenderer& renderer)
{
  // check if render data is invalid
  if (m_renderDataNeedsUpdate)
  {
    // update render data
    updateRenderData();

    // reset flag
    m_renderDataNeedsUpdate = false;
  }

  return renderer.addForRendering(m_renderData);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void SpriteAnimation::setBaseAlignment(Alignment alignment)
{
  if (m_baseAlignment != alignment)
  {
    m_baseAlignment = alignment;

    // make sure render data gets updated
    m_renderDataNeedsUpdate = true;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void SpriteAnimation::setAlpha(float32 alpha)
{
  if (m_alpha != alpha)
  {
    m_alpha = alpha;

    // make sure render data gets updated
    m_renderDataNeedsUpdate = true;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void SpriteAnimation::setRenderPriority(s32 priority)
{
  m_renderData->setPriority(priority);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void SpriteAnimation::setTransformationMatrix(const Matrix4f& transform)
{
  if (m_transform != transform)
  {
    // store new value
    m_transform = transform;

    // mark dirty to recalculate
    m_renderDataNeedsUpdate = true;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void SpriteAnimation::updateRenderData()
{
  EGE_ASSERT(NULL != m_renderData->material());
  EGE_ASSERT(1 == m_renderData->material()->passCount());
  EGE_ASSERT(1 == m_renderData->material()->pass(0)->textureCount());

  const u32 vertexCount = m_renderData->vertexBuffer()->vertexCount();
  EGE_ASSERT(6 == vertexCount);

  float32* data = reinterpret_cast<float32*>(m_renderData->vertexBuffer()->lock(0, vertexCount));
  EGE_ASSERT(NULL != data);

  Matrix4f finalMatrix = m_transform * m_physicsData->transformationMatrix();

  // NOTE: size AFTER scaling is required for proper realignments while for vertex calculations size BEFORE scaling should be used 
  //       (vertices will be converted by transformation matrix)
  const Vector2f size = m_displaySize;
  const Vector2f alignmentSize = size * Vector2f(finalMatrix.scaleX(), finalMatrix.scaleY());

  // apply alignment
  Vector2f translation = Vector2f(finalMatrix.translationX(), finalMatrix.translationY());
  translation = Math::Align(translation, alignmentSize, ALIGN_TOP_LEFT, m_baseAlignment);
  finalMatrix.setTranslation(translation.x, translation.y, finalMatrix.translationZ());

  // get texture coordinates
  const Rectf& textureRect = m_frameData[m_currentSequencer->frameId(m_currentSequencer->currentFrameIndex())].m_rect;

  // calculate quad vertices
  Vector4f vertexTL(0, 0, 0, 1);
  Vector4f vertexBL(0, size.y, 0, 1);
  Vector4f vertexBR(size.x, size.y, 0, 1);
  Vector4f vertexTR(size.x, 0, 0, 1);

  vertexTL = finalMatrix * vertexTL;
  vertexBL = finalMatrix * vertexBL;
  vertexBR = finalMatrix * vertexBR;
  vertexTR = finalMatrix * vertexTR;

  // update render data

  // vertex 1
  *data++ = vertexTL.x;
  *data++ = vertexTL.y;
  *data++ = textureRect.x;
  *data++ = textureRect.y;
  *data++ = 1;
  *data++ = 1;
  *data++ = 1;
  *data++ = m_alpha;

  // vertex 2
  *data++ = vertexBL.x;
  *data++ = vertexBL.y;
  *data++ = textureRect.x;
  *data++ = textureRect.y + textureRect.height;
  *data++ = 1;
  *data++ = 1;
  *data++ = 1;
  *data++ = m_alpha;

  // vertex 3
  *data++ = vertexBR.x;
  *data++ = vertexBR.y;
  *data++ = textureRect.x + textureRect.width;
  *data++ = textureRect.y + textureRect.height;
  *data++ = 1;
  *data++ = 1;
  *data++ = 1;
  *data++ = m_alpha;

  // vertex 4
  *data++ = vertexTL.x;
  *data++ = vertexTL.y;
  *data++ = textureRect.x;
  *data++ = textureRect.y;
  *data++ = 1;
  *data++ = 1;
  *data++ = 1;
  *data++ = m_alpha;

  // vertex 5
  *data++ = vertexBR.x;
  *data++ = vertexBR.y;
  *data++ = textureRect.x + textureRect.width;
  *data++ = textureRect.y + textureRect.height;
  *data++ = 1;
  *data++ = 1;
  *data++ = 1;
  *data++ = m_alpha;

  // vertex 6
  *data++ = vertexTR.x;
  *data++ = vertexTR.y;
  *data++ = textureRect.x + textureRect.width;
  *data++ = textureRect.y;
  *data++ = 1;
  *data++ = 1;
  *data++ = 1;
  *data++ = m_alpha;

  m_renderData->vertexBuffer()->unlock(data - 1);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void SpriteAnimation::setDisplaySize(const Vector2f& size)
{
  if (m_displaySize != size)
  {
    m_displaySize = size;

    // mark dirty to recalculate
    m_renderDataNeedsUpdate = true;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const Vector2f& SpriteAnimation::displaySize() const
{
  return m_displaySize;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void SpriteAnimation::onTransformationChanged()
{
  m_renderDataNeedsUpdate = true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void SpriteAnimation::onSequencerFrameChanged(s32 frameId)
{
  // call base class
  KeyFrameAnimation::onSequencerFrameChanged(frameId);

  // invalidate render data
  m_renderDataNeedsUpdate = true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
