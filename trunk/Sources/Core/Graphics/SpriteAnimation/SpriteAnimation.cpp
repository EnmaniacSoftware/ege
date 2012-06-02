#include "Core/Graphics/SpriteAnimation/SpriteAnimation.h"
#include "Core/Resource/ResourceSpritesheet.h"
#include "Core/Resource/ResourceTextureImage.h"
#include <EGEPhysics.h>
#include <EGEGraphics.h>
#include <EGEDebug.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(SpriteAnimation)
EGE_DEFINE_DELETE_OPERATORS(SpriteAnimation)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//SpriteAnimation::SpriteAnimation() : Object(NULL), 
//                                     m_state(STATE_STOPPED), 
//                                     m_name("")
//{
//}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SpriteAnimation::SpriteAnimation(Application* app, const String& name) : Object(app), 
                                                                         m_state(STATE_STOPPED), 
                                                                         m_name(name),
                                                                         m_baseAlignment(ALIGN_TOP_LEFT)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SpriteAnimation::~SpriteAnimation()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Constructs objects. */
EGEResult SpriteAnimation::construct()
{
  EGEResult result;

  // create render data
  m_renderData = RenderObjectFactory::CreateQuadXY(app(), name(), Vector4f::ZERO, Vector2f::ONE, ALIGN_TOP_LEFT, EGEVertexBuffer::ST_V2_T2, 
                                                   EGEGraphics::RP_MAIN, EGEGraphics::RPT_TRIANGLE_STRIPS, EGEVertexBuffer::UT_STATIC_WRITE);
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

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IAnimation override. Starts playback with a given sequencer. 
 *  @param sequencerName  Name of the sequencer to use for playback.
 *  @note If animation for given sequencer is was paused it will be resumed. Otherwise, animation will be started from the begining.
 */
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
/*! IAnimation override. Starts playback with a given sequencer. 
 *  @param sequencerIndex Index of the sequencer to use for playback. Negative value replays last sequence if available.
 *  @note If animation for given sequencer is was paused it will be resumed. Otherwise, animation will be started from the begining.
 */
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
/* IAnimation override. Stops playback. */
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
/* IAnimation override. Pauses playback. */
void SpriteAnimation::pause()
{
  // change state
  m_state = STATE_PAUSED;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IAnimation override. Returns TRUE if animation is being played. */
bool SpriteAnimation::isPlaying() const
{
  return STATE_PLAYING == m_state;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IAnimation override. Returns TRUE if animation is paused. */
bool SpriteAnimation::isPaused() const
{
  return STATE_PAUSED == m_state;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IAnimation override. Returns TRUE if animation is stopped. */
bool SpriteAnimation::isStopped() const
{
  return STATE_STOPPED == m_state;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IAnimation override. Updates animation. */
void SpriteAnimation::update(const Time& time)
{
  if (isPlaying())
  {
    m_currentSequencer->update(time);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns texture image for current frame. */
PTextureImage SpriteAnimation::frameTexture() const
{
  EGE_ASSERT(NULL != m_currentSequencer);

  m_textureImage->setRect(m_frameData[m_currentSequencer->frameId(m_currentSequencer->currentFrameIndex())].m_rect);
  return m_textureImage;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets FPS playback value. */
void SpriteAnimation::setFPS(float32 fps)
{
  m_frameDuration = (0.0f < fps) ? (1.0f / fps) : 0.0f;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets frame data. */
void SpriteAnimation::setFrameData(const DynamicArray<EGESprite::FrameData>& data)
{
  m_frameData.copy(data);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets texture image containing sprite data. */
void SpriteAnimation::setTexture(const PTextureImage& texture)
{
  m_textureImage = texture;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets name. */
void SpriteAnimation::setName(const String& name)
{
  m_name = name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Adds sequencer. */
void SpriteAnimation::addSequencer(const PSequencer& sequencer)
{
  // check if sequencer with such name exists
  if (NULL != this->sequencer(sequencer->name()))
  {
    // cannot add
    EGE_WARNING("Sequencer with name: %s already exists", sequencer->name().toAscii());
    return;
  }

  // add to pool
  m_sequencers.push_back(sequencer);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns sequencer of a given name. */
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
/*! Slot called when sequencer animated into new frame. */
void SpriteAnimation::onSequencerFrameChanged(PSequencer sequencer, s32 frameId)
{
  EGE_ASSERT(NULL != m_renderData);

  // update render data
  m_renderData->material()->pass(0)->setTexture(0, frameTexture());

  // emit
  emit frameChanged(this, frameId);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when sequencer finished animation .*/
void SpriteAnimation::onSequencerFinished(PSequencer sequencer)
{
  // stop
  m_state = STATE_STOPPED;

  // emit
  emit finished(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns current sequencer. */
PSequencer SpriteAnimation::currentSequencer() const 
{ 
  return m_currentSequencer; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Renders animation. */
void SpriteAnimation::addForRendering(Renderer* renderer, const Matrix4f& transform)
{
  // apply alignment
  Matrix4f matrix       = Matrix4f::IDENTITY;
  Vector4f translation  = m_physicsData->position();
  Vector4f scale        = m_physicsData->scale();

  Math::Align(&translation, &scale, m_baseAlignment, ALIGN_TOP_LEFT);
  Math::CreateMatrix(&matrix, &translation, &scale, &Quaternionf::IDENTITY);

  renderer->addForRendering(m_renderData, transform * matrix);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets base display alignment. 
 *  @param alignment Alignment animation is originally created for.
 *  @note  Animation if always aligned to TOP_LEFT anchor from its base alignment.
 */
void SpriteAnimation::setBaseAlignment(Alignment alignment)
{
  m_baseAlignment = alignment;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END
