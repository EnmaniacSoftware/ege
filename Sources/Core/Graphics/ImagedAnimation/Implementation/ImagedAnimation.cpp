#include "Core/Graphics/ImagedAnimation/Interface/ImagedAnimation.h"
#include "EGESequencer.h"
#include "EGEPhysics.h"
#include "EGEGraphics.h"
#include "EGERenderComponent.h"
#include "EGERenderer.h"
#include "EGEDebug.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const char* KImagedAnimationDebugName = "EGEImagedAnimation";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ImagedAnimation::ImagedAnimation(Engine& engine) 
: KeyFrameAnimation(engine, "")
, m_renderPriority(EGEGraphics::RP_MAIN)
, m_displaySize(50, 50)
, m_baseAlignment(ALIGN_TOP_LEFT)
, m_alpha(1.0f)
, m_renderDataNeedsUpdate(false)
, m_transform(Matrix4f::IDENTITY)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ImagedAnimation::ImagedAnimation(Engine& engine, const String& name) 
: KeyFrameAnimation(engine, name)
, m_renderPriority(EGEGraphics::RP_MAIN)
, m_displaySize(50, 50)
, m_baseAlignment(ALIGN_TOP_LEFT)
, m_alpha(1.0f)
, m_renderDataNeedsUpdate(false)
, m_transform(Matrix4f::IDENTITY)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ImagedAnimation::~ImagedAnimation()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ImagedAnimation::play(const String& sequencerName)
{
  EGEResult result = EGE_ERROR;

  // check if appropriate animation data is present
  if ( ! m_objects.empty() && ! m_framesActionsList.isEmpty())
  {
    // try to start animation
    result = KeyFrameAnimation::play(sequencerName);
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ImagedAnimation::addForRendering(IRenderer& renderer)
{
  EGEResult result = EGE_SUCCESS;

  // check if no sequencer
  if (NULL != m_currentSequencer)
  {
    // check if render data is invalid
    if (m_renderDataNeedsUpdate)
    {
      // update render data
      updateRenderData();

      // reset flag
      m_renderDataNeedsUpdate = false;
    }

    EGE_ASSERT(m_currentSequencer->frameId(m_currentSequencer->currentFrameIndex()) < static_cast<s32>(m_renderComponents.size()));

    // get current frame render data
    PRenderComponent& renderComponent = m_renderComponents[m_currentSequencer->frameId(m_currentSequencer->currentFrameIndex())];

    // update alpha
    //ColorTransform colorTransform(Color::NONE, Color(1.0f, 1.0f, 1.0f, m_alpha));
    //frameData.renderData->material()->setDiffuseColorTransformation(colorTransform);

    // render
    renderComponent->setPriority(m_renderPriority);
    result = renderer.addForRendering(renderComponent);
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ImagedAnimation::setRenderPriority(s32 priority)
{
  m_renderPriority = priority;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ImagedAnimation::setDisplaySize(const Vector2f& size)
{
  m_displaySize = size;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const Vector2f& ImagedAnimation::displaySize() const
{
  return m_displaySize;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ImagedAnimation::setBaseAlignment(Alignment alignment)
{
  if (m_baseAlignment != alignment)
  {
    m_baseAlignment = alignment;

    // make sure render data gets updated
    m_renderDataNeedsUpdate = true;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ImagedAnimation::clear()
{
  stop();

  m_objects.clear();
  m_renderComponents.clear();
  m_sequencers.clear();
  m_currentSequencer = NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ImagedAnimation::setAlpha(float32 alpha)
{
  m_alpha = alpha;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ImagedAnimation::updateRenderData()
{
  // TAGE - possibly we can reuse existing data (if any) in case of ie transformation changes
  m_renderComponents.clear();

  VertexDeclaration declaration;
  declaration.addElement(NVertexBuffer::VES_POSITION_XY);
  declaration.addElement(NVertexBuffer::VES_TEXTURE_UV);
  declaration.addElement(NVertexBuffer::VES_COLOR_RGBA);

  // go thru all frames
  for (List<EGEImagedAnimation::ActionDataList>::ConstIterator itFrame = m_framesActionsList.begin(); itFrame != m_framesActionsList.end(); ++itFrame)
  {
    const List<EGEImagedAnimation::ActionData>& actions = *itFrame;

    // calculate number of vertices required for current frame
    const u32 vertexCount = calculateFrameVertexCount(m_objects, actions);
    
    // create render component for current frame
    PRenderComponent frameRenderData = ege_new RenderComponent(engine(), name(), declaration);
    EGE_ASSERT(NULL != frameRenderData);

    PVertexBuffer vertexBuffer = frameRenderData->vertexBuffer();
    vertexBuffer->setSize(vertexCount);
    float32* data = reinterpret_cast<float32*>(vertexBuffer->lock(0, vertexCount));
    EGE_ASSERT(NULL != data);

    // go thru all frame actions
    for (List<EGEImagedAnimation::ActionData>::ConstIterator itAction = actions.begin(); itAction != actions.end(); ++itAction)
    {
      const EGEImagedAnimation::ActionData& action = *itAction;
      
      // go thru all action children
      const EGEImagedAnimation::Object& object = m_objects.at(action.objectId);
      for (EGEImagedAnimation::ChildObjectList::ConstIterator itObject = object.children.begin(); itObject != object.children.end(); ++itObject)
      {
        const EGEImagedAnimation::ChildObject& child = *itObject;

        Matrix4f finalMatrix = m_transform * (action.matrix * child.matrix);
        
        // check if no material assigned yet
        if (frameRenderData->material() == NULL)
        {
          // create material
          PMaterial material = ege_new Material();

          // add pass
          RenderPass* pass = material->addPass(NULL);

          // setup material
          pass->setSrcBlendFactor(EGEGraphics::BF_SRC_ALPHA);
          pass->setDstBlendFactor(EGEGraphics::BF_ONE_MINUS_SRC_ALPHA);

          PTextureImage ti = ege_new TextureImage(child.material->pass(0)->texture(0)->texture());
          pass->addTexture(ti);

          frameRenderData->setMaterial(material);
        }

        // apply alignment
        Vector2f translation = Vector2f(finalMatrix.translationX(), finalMatrix.translationY());
        translation = Math::Align(translation, m_displaySize, ALIGN_TOP_LEFT, m_baseAlignment);
        finalMatrix.setTranslation(translation.x, translation.y, finalMatrix.translationZ());

        // calculate quad vertices
        Vector4f vertexTL(0, 0, 0, 1);
        Vector4f vertexBL(0, child.size.y, 0, 1);
        Vector4f vertexBR(child.size.x, child.size.y, 0, 1);
        Vector4f vertexTR(child.size.x, 0, 0, 1);

        vertexTL = finalMatrix * vertexTL;
        vertexBL = finalMatrix * vertexBL;
        vertexBR = finalMatrix * vertexBR;
        vertexTR = finalMatrix * vertexTR;

        // update render data

        // vertex 1
        *data++ = vertexTL.x;
        *data++ = vertexTL.y;
        *data++ = child.rect.x;
        *data++ = child.rect.y;
        *data++ = action.color.red;
        *data++ = action.color.green;
        *data++ = action.color.blue;
        *data++ = action.color.alpha;

        // vertex 2
        *data++ = vertexBL.x;
        *data++ = vertexBL.y;
        *data++ = child.rect.x;
        *data++ = child.rect.y + child.rect.height;
        *data++ = action.color.red;
        *data++ = action.color.green;
        *data++ = action.color.blue;
        *data++ = action.color.alpha;

        // vertex 3
        *data++ = vertexBR.x;
        *data++ = vertexBR.y;
        *data++ = child.rect.x + child.rect.width;
        *data++ = child.rect.y + child.rect.height;
        *data++ = action.color.red;
        *data++ = action.color.green;
        *data++ = action.color.blue;
        *data++ = action.color.alpha;

        // vertex 4
        *data++ = vertexTL.x;
        *data++ = vertexTL.y;
        *data++ = child.rect.x;
        *data++ = child.rect.y;
        *data++ = action.color.red;
        *data++ = action.color.green;
        *data++ = action.color.blue;
        *data++ = action.color.alpha;

        // vertex 5
        *data++ = vertexBR.x;
        *data++ = vertexBR.y;
        *data++ = child.rect.x + child.rect.width;
        *data++ = child.rect.y + child.rect.height;
        *data++ = action.color.red;
        *data++ = action.color.green;
        *data++ = action.color.blue;
        *data++ = action.color.alpha;

        // vertex 6
        *data++ = vertexTR.x;
        *data++ = vertexTR.y;
        *data++ = child.rect.x + child.rect.width;
        *data++ = child.rect.y;
        *data++ = action.color.red;
        *data++ = action.color.green;
        *data++ = action.color.blue;
        *data++ = action.color.alpha;
      }
    }

    vertexBuffer->unlock(data - 1);

    // add to pool
    m_renderComponents.push_back(frameRenderData);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ImagedAnimation::addData(const EGEImagedAnimation::ObjectMap& objects, const List<EGEImagedAnimation::ActionDataList>& frameActions)
{
  // store data
  m_objects           = objects;
  m_framesActionsList = frameActions;

  // make sure render data gets rebuilt
  m_renderDataNeedsUpdate = true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
u32 ImagedAnimation::calculateFrameVertexCount(const EGEImagedAnimation::ObjectMap& objects, const EGEImagedAnimation::ActionDataList& frameActionList) const
{
  u32 vertexCount = 0;

  // go thru all frame actions
  for (EGEImagedAnimation::ActionDataList::ConstIterator itAction = frameActionList.begin(); itAction != frameActionList.end(); ++itAction)
  {
    const EGEImagedAnimation::ActionData& action = *itAction;
      
    // go thru all action children
    const EGEImagedAnimation::Object& object = objects.at(action.objectId);

    // update vertex count (number of objects)
    vertexCount += object.children.length();
  }

  // (6 vertices per object/quad)
  return vertexCount * 6;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ImagedAnimation::setTransformationMatrix(const Matrix4f& transform)
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
