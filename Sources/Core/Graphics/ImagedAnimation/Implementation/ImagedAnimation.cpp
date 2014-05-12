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
EGE_DEFINE_NEW_OPERATORS(ImagedAnimation)
EGE_DEFINE_DELETE_OPERATORS(ImagedAnimation)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ImagedAnimation::ImagedAnimation() : Object(NULL)
                                   , m_state(STATE_STOPPED)
                                   , m_name("")
                                   , m_renderPriority(EGEGraphics::RP_MAIN)
                                   , m_displaySize(50, 50)
                                   , m_baseAlignment(ALIGN_TOP_LEFT)
                                   , m_alpha(1.0f)
                                   , m_renderDataNeedsUpdate(false)
                                   , m_transform(Matrix4f::IDENTITY)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ImagedAnimation::ImagedAnimation(Application* app, const String& name) : Object(app)
                                                                       , m_state(STATE_STOPPED)
                                                                       , m_name(name)
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
  // check if resumed
  if (isPaused())
  {
    // check if resuming the same sequencer
    if ((NULL != m_currentSequencer) && (sequencerName == m_currentSequencer->name()))
    {
      // change state
      m_state = STATE_PLAYING;
    }
    else
    {
      // stop it so it can be restarted from the begining
      stop();
    }
  }
  
  // get sequencer
  m_currentSequencer = this->sequencer(sequencerName);
  if (NULL == m_currentSequencer)
  {
    // error!
    return EGE_ERROR_NOT_FOUND;
  }

  if ( ! isPlaying() && ! m_objects.empty() && ! m_framesActionsList.empty() && (NULL != m_currentSequencer))
  {
    // setup sequencer
    m_currentSequencer->setFrameDuration(m_frameDuration);

    // connect to sequencer
    ege_connect(m_currentSequencer, frameChanged, this, ImagedAnimation::onSequencerFrameChanged);
    ege_connect(m_currentSequencer, finished, this, ImagedAnimation::onSequencerFinished);

    // change state
    m_state = STATE_PLAYING;

    // start sequencer
    m_currentSequencer->start();
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ImagedAnimation::play(s32 sequencerIndex)
{
  // check if replay requested
  if (0 > sequencerIndex)
  {
    // check if replay possible
    if (NULL != m_currentSequencer)
    {
      // replay
      return play(m_currentSequencer->name());
    }

    return EGE_ERROR_NOT_FOUND;
  }

  PSequencer seq = m_sequencers.at(sequencerIndex, NULL);
  if (NULL == seq)
  {
    // error!
    return EGE_ERROR_NOT_FOUND;
  }

  return play(seq->name());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ImagedAnimation::stop()
{
  if (isPlaying())
  {
    // disconnect from sequencer
    ege_disconnect(m_currentSequencer, frameChanged, this, ImagedAnimation::onSequencerFrameChanged);
    ege_disconnect(m_currentSequencer, finished, this, ImagedAnimation::onSequencerFinished);

    // change state
    m_state = STATE_STOPPED;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ImagedAnimation::pause()
{
  // change state
  m_state = STATE_PAUSED;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ImagedAnimation::isPlaying() const
{
  return STATE_PLAYING == m_state;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ImagedAnimation::isPaused() const
{
  return STATE_PAUSED == m_state;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ImagedAnimation::isStopped() const
{
  return STATE_STOPPED == m_state;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ImagedAnimation::update(const Time& time)
{
  if (isPlaying())
  {
    // update sequencer
    m_currentSequencer->update(time);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ImagedAnimation::setFPS(float32 fps)
{
  m_frameDuration = (0.0f < fps) ? (1.0f / fps) : 0.0f;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ImagedAnimation::setName(const String& name)
{
  m_name = name;
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
  m_objects.clear();
  m_renderComponents.clear();
  m_sequencers.clear();

  // disconnect from sequencer
  if (NULL != m_currentSequencer)
  {
    ege_disconnect(m_currentSequencer, frameChanged, this, ImagedAnimation::onSequencerFrameChanged);
    ege_disconnect(m_currentSequencer, finished, this, ImagedAnimation::onSequencerFinished);
  }

  m_currentSequencer = NULL;
  m_state            = STATE_STOPPED;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ImagedAnimation::addSequencer(const PSequencer& sequencer)
{
  // check if sequencer with such name exists
  if (NULL != this->sequencer(sequencer->name()))
  {
    // cannot add
    egeWarning(KImagedAnimationDebugName) << "Sequencer with name:" << sequencer->name() << "already exists";
    return;
  }

  // add to pool
  m_sequencers.push_back(sequencer);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PSequencer ImagedAnimation::sequencer(const String& name) const
{
  // go thru all sequencers
  for (SequencerArray::const_iterator it = m_sequencers.begin(); it != m_sequencers.end(); ++it)
  {
    const PSequencer& current = *it;
    if (current->name() == name)
    {
      // found
      return current;
    }
  }

  // not found
  return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ImagedAnimation::onSequencerFrameChanged(PSequencer sequencer, s32 frameId)
{
  // emit
  emit frameChanged(this, frameId);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ImagedAnimation::onSequencerFinished(PSequencer sequencer)
{
  // stop
  m_state = STATE_STOPPED;

  // emit
  emit finished(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PSequencer ImagedAnimation::currentSequencer() const 
{ 
  return m_currentSequencer; 
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
  for (List<EGEImagedAnimation::ActionDataList>::const_iterator itFrame = m_framesActionsList.begin(); itFrame != m_framesActionsList.end(); ++itFrame)
  {
    const List<EGEImagedAnimation::ActionData>& actions = *itFrame;

    // calculate number of vertices required for current frame
    const u32 vertexCount = calculateFrameVertexCount(m_objects, actions);
    
    // create render component for current frame
    PRenderComponent frameRenderData = ege_new RenderComponent(app(), name(), declaration);
    EGE_ASSERT(NULL != frameRenderData);

    PVertexBuffer vertexBuffer = frameRenderData->vertexBuffer();
    vertexBuffer->setSize(vertexCount);
    float32* data = reinterpret_cast<float32*>(vertexBuffer->lock(0, vertexCount));
    EGE_ASSERT(NULL != data);

    // go thru all frame actions
    for (List<EGEImagedAnimation::ActionData>::const_iterator itAction = actions.begin(); itAction != actions.end(); ++itAction)
    {
      const EGEImagedAnimation::ActionData& action = *itAction;
      
      // go thru all action children
      const EGEImagedAnimation::Object& object = m_objects.at(action.objectId);
      for (EGEImagedAnimation::ChildObjectList::const_iterator itObject = object.children.begin(); itObject != object.children.end(); ++itObject)
      {
        const EGEImagedAnimation::ChildObject& child = *itObject;

        Matrix4f finalMatrix = m_transform * (action.matrix * child.matrix);
        
        // check if no material assigned yet
        if (frameRenderData->material() == NULL)
        {
          // create material
          PMaterial material = ege_new Material(app());

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
  for (EGEImagedAnimation::ActionDataList::const_iterator itAction = frameActionList.begin(); itAction != frameActionList.end(); ++itAction)
  {
    const EGEImagedAnimation::ActionData& action = *itAction;
      
    // go thru all action children
    const EGEImagedAnimation::Object& object = objects.at(action.objectId);

    // update vertex count (number of objects)
    vertexCount += object.children.size();
  }

  // (6 vertices per object/quad)
  return vertexCount * 6;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const String& ImagedAnimation::name() const
{ 
  return m_name; 
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
