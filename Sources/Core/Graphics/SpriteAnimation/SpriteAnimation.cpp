#include "Core/Graphics/SpriteAnimation/SpriteAnimation.h"
#include "Core/Resource/ResourceSpritesheet.h"
#include "Core/Resource/ResourceTextureImage.h"
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
EGE_DEFINE_NEW_OPERATORS(SpriteAnimation)
EGE_DEFINE_DELETE_OPERATORS(SpriteAnimation)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SpriteAnimation::SpriteAnimation(Application* app, const String& name) : Object(app) 
                                                                       , m_state(STATE_STOPPED)
                                                                       , m_name(name)
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
  EGEResult result;

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

  PTextureImage texture = ege_new TextureImage(app());
  if (NULL == texture)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  texture->setName("sprite-texture");
  if (EGE_SUCCESS != (result = pass->addTexture(texture)))
  {
    // error!
    return result;
  }

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
EGEResult SpriteAnimation::play(const String& sequencerName)
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

  if (!isPlaying() && !m_frameData.empty() && (NULL != m_currentSequencer))
  {
    // setup sequencer
    m_currentSequencer->setFrameDuration(m_frameDuration);

    // connect to sequencer
    ege_connect(m_currentSequencer, frameChanged, this, SpriteAnimation::onSequencerFrameChanged);
    ege_connect(m_currentSequencer, finished, this, SpriteAnimation::onSequencerFinished);

    // change state
    m_state = STATE_PLAYING;

    // start sequencer
    m_currentSequencer->start();
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SpriteAnimation::play(s32 sequencerIndex)
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
void SpriteAnimation::stop()
{
  if (isPlaying())
  {
    // disconnect from sequencer
    ege_disconnect(m_currentSequencer, frameChanged, this, SpriteAnimation::onSequencerFrameChanged);
    ege_disconnect(m_currentSequencer, finished, this, SpriteAnimation::onSequencerFinished);

    // change state
    m_state = STATE_STOPPED;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void SpriteAnimation::pause()
{
  // change state
  m_state = STATE_PAUSED;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool SpriteAnimation::isPlaying() const
{
  return STATE_PLAYING == m_state;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool SpriteAnimation::isPaused() const
{
  return STATE_PAUSED == m_state;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool SpriteAnimation::isStopped() const
{
  return STATE_STOPPED == m_state;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void SpriteAnimation::update(const Time& time)
{
  if (isPlaying())
  {
    m_currentSequencer->update(time);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PTextureImage SpriteAnimation::frameTexture() const
{
  EGE_ASSERT(NULL != m_currentSequencer);

  m_textureImage->setRect(m_frameData[m_currentSequencer->frameId(m_currentSequencer->currentFrameIndex())].m_rect);
  return m_textureImage;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void SpriteAnimation::setFPS(float32 fps)
{
  m_frameDuration = (0.0f < fps) ? (1.0f / fps) : 0.0f;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void SpriteAnimation::setFrameData(const DynamicArray<EGESprite::FrameData>& data)
{
  m_frameData.copy(data);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void SpriteAnimation::setTexture(const PTextureImage& texture)
{
  m_textureImage = texture;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void SpriteAnimation::setName(const String& name)
{
  m_name = name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void SpriteAnimation::addSequencer(const PSequencer& sequencer)
{
  // check if sequencer with such name exists
  if (NULL != this->sequencer(sequencer->name()))
  {
    // cannot add
    egeWarning(KSpriteAnimationDebugName) << "Sequencer with name:" << sequencer->name() << "already exists";
    return;
  }

  // add to pool
  m_sequencers.push_back(sequencer);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PSequencer SpriteAnimation::sequencer(const String& name) const
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
void SpriteAnimation::onSequencerFrameChanged(PSequencer sequencer, s32 frameId)
{
  EGE_ASSERT(NULL != m_renderData);

  // update render data
  m_renderData->material()->pass(0)->setTexture(0, frameTexture());

  // emit
  emit frameChanged(this, frameId);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void SpriteAnimation::onSequencerFinished(PSequencer sequencer)
{
  // stop
  m_state = STATE_STOPPED;

  // emit
  emit finished(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PSequencer SpriteAnimation::currentSequencer() const 
{ 
  return m_currentSequencer; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SpriteAnimation::addForRendering(IRenderer& renderer)
{
  EGEResult result = EGE_SUCCESS;

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
  const u32 vertexCount = m_renderData->vertexBuffer()->vertexCount();
  EGE_ASSERT(6 == vertexCount);

  float32* data = reinterpret_cast<float32*>(m_renderData->vertexBuffer()->lock(0, vertexCount));
  EGE_ASSERT(NULL != data);

  Matrix4f finalMatrix = m_transform * m_physicsData->transformationMatrix();

  // apply alignment
  Vector2f translation = Vector2f(finalMatrix.translationX(), finalMatrix.translationY());
  translation = Math::Align(translation, m_displaySize, ALIGN_TOP_LEFT, m_baseAlignment);

  finalMatrix.setTranslation(translation.x, translation.y, finalMatrix.translationZ());

  // calculate final size
  const Vector2f size = m_displaySize * Vector2f(finalMatrix.scaleX(), finalMatrix.scaleY());

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
  *data++ = 0;
  *data++ = 0;
  *data++ = 1;
  *data++ = 1;
  *data++ = 1;
  *data++ = m_alpha;

  // vertex 2
  *data++ = vertexBL.x;
  *data++ = vertexBL.y;
  *data++ = 0;
  *data++ = 1;
  *data++ = 1;
  *data++ = 1;
  *data++ = 1;
  *data++ = m_alpha;

  // vertex 3
  *data++ = vertexBR.x;
  *data++ = vertexBR.y;
  *data++ = 1;
  *data++ = 1;
  *data++ = 1;
  *data++ = 1;
  *data++ = 1;
  *data++ = m_alpha;

  // vertex 4
  *data++ = vertexTL.x;
  *data++ = vertexTL.y;
  *data++ = 0;
  *data++ = 0;
  *data++ = 1;
  *data++ = 1;
  *data++ = 1;
  *data++ = m_alpha;

  // vertex 5
  *data++ = vertexBR.x;
  *data++ = vertexBR.y;
  *data++ = 1;
  *data++ = 1;
  *data++ = 1;
  *data++ = 1;
  *data++ = 1;
  *data++ = m_alpha;

  // vertex 6
  *data++ = vertexTR.x;
  *data++ = vertexTR.y;
  *data++ = 1;
  *data++ = 0;
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
