#include "Core/Animation/ImagedAnimation.h"
#include "Core/Resource/ResourceImagedAnimation.h"
#include "Core/Resource/ResourceTextureImage.h"
#include <EGESequencer.h>
#include <EGEPhysics.h>
#include <EGEGraphics.h>
#include <EGEDebug.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ImagedAnimation)
EGE_DEFINE_DELETE_OPERATORS(ImagedAnimation)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ImagedAnimation::ImagedAnimation() : Object(NULL), 
                                     m_state(STATE_STOPPED), 
                                     m_name(""),
                                     m_baseRenderPriority(EGEGraphics::RP_MAIN),
                                     m_displaySize(50, 50),
                                     m_baseAlignment(ALIGN_TOP_LEFT)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ImagedAnimation::ImagedAnimation(Application* app, const String& name) : Object(app), 
                                                                         m_state(STATE_STOPPED), 
                                                                         m_name(name),
                                                                         m_baseRenderPriority(EGEGraphics::RP_MAIN),
                                                                         m_displaySize(50, 50),
                                                                         m_baseAlignment(ALIGN_TOP_LEFT)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ImagedAnimation::~ImagedAnimation()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IAnimation override. Starts playback with a given sequencer. 
 *  @param sequencerName  Name of the sequencer to use for playback.
 *  @note If animation for given sequencer is was paused it will be resumed. Otherwise, animation will be started from the begining.
 */
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

  if (!isPlaying() && !m_objects.empty() && !m_frames.empty() && (NULL != m_currentSequencer))
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
/* IAnimation override. Stops playback. */
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
/* IAnimation override. Pauses playback. */
void ImagedAnimation::pause()
{
  // change state
  m_state = STATE_PAUSED;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IAnimation override. Returns TRUE if animation is being played. */
bool ImagedAnimation::isPlaying() const
{
  return STATE_PLAYING == m_state;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IAnimation override. Returns TRUE if animation is paused. */
bool ImagedAnimation::isPaused() const
{
  return STATE_PAUSED == m_state;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IAnimation override. Returns TRUE if animation is stopped. */
bool ImagedAnimation::isStopped() const
{
  return STATE_STOPPED == m_state;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IAnimation override. Updates animation. */
void ImagedAnimation::update(const Time& time)
{
  if (isPlaying())
  {
    m_currentSequencer->update(time);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets frame duration. */
void ImagedAnimation::setFrameDuration(const Time& duration)
{
  m_frameDuration = duration;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets name. */
void ImagedAnimation::setName(const String& name)
{
  m_name = name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Adds object with a given id to animation. 
 *  @param objectId    Object Id which is being added.
 *  @param size        Object size (in pixels).
 *  @param material    Material used by the object.
 *  @param baseMatrix  Object's base transformation matrix.
 */
EGEResult ImagedAnimation::addObject(s32 objectId, const Vector2f& size, PMaterial material, const Matrix4f& baseMatrix)
{
  ObjectData data;

  // check if object already in
  if (m_objects.contains(objectId))
  {
    // error!
    return EGE_ERROR_ALREADY_EXISTS;
  }

  // create object
  data.baseFrameMatrix  = Matrix4f::IDENTITY;
  data.baseMatrix       = baseMatrix;
  data.renderData       = RenderObjectFactory::CreateQuadXY(app(), String::Format("image-animation-object-%d", m_objects.size()), Vector4f::ZERO, size,
                                                            ALIGN_TOP_LEFT, EGEVertexBuffer::ST_V2_T2, 0, EGEGraphics::RPT_TRIANGLE_STRIPS, 
                                                            EGEVertexBuffer::UT_DYNAMIC_WRITE_DONT_CARE);

  // check if properly created
  if ((NULL != data.renderData))
  {
    // set material
    data.renderData->setMaterial(material);

    // add into pool
    m_objects.insert(objectId, data);
    return EGE_SUCCESS;
  }

  return EGE_ERROR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Adds frame data.
 *  @param action  List of action to be processed at given frame.
 *  @note  This creates new frame and appends it into existing ones.
 */
EGEResult ImagedAnimation::addFrameData(const List<EGEImagedAnimation:: ActionData>& actions)
{
  FrameData frameData;
  frameData.actions = actions;

  m_frames.push_back(frameData);

  // if first frame added, update object
  if (1 == m_frames.size())
  {
   // update(0);
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Renders animation. */
void ImagedAnimation::addForRendering(Renderer* renderer, const Matrix4f& transform)
{
  if (NULL != m_currentSequencer)
  {
    const FrameData& frameData = m_frames[m_currentSequencer->currentFrameId()];
    for (List<EGEImagedAnimation::ActionData>::const_iterator it = frameData.actions.begin(); it != frameData.actions.end(); ++it)
    {
      const EGEImagedAnimation::ActionData& action = *it;

      const ObjectData& objectData = m_objects.at(action.objectId);
      renderer->addForRendering(objectData.renderData, transform * objectData.baseFrameMatrix);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets base render priority. */
void ImagedAnimation::setBaseRenderPriority(s32 priority)
{
  m_baseRenderPriority = priority;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets display size. */
void ImagedAnimation::setDisplaySize(const Vector2f& size)
{
  m_displaySize = size;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets base display alignment. 
 *  @param alignment Alignment animation is originally created for.
 *  @note  Animation if always aligned to TOP_LEFT anchor from its base alignment.
 */
void ImagedAnimation::setBaseAlignment(Alignment alignment)
{
  m_baseAlignment = alignment;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Clears object. */
void ImagedAnimation::clear()
{
  m_objects.clear();
  m_frames.clear();
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
/*! Adds sequencer. */
void ImagedAnimation::addSequencer(const PSequencer& sequencer)
{
  // check if sequencer with such name exists
  if (NULL != this->sequencer(sequencer->name()))
  {
    // cannot add
    EGE_WARNING("Sequencer with name: %s already exists", sequencer->name());
    return;
  }

  // add to pool
  m_sequencers.push_back(sequencer);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns sequencer of a given name. */
PSequencer ImagedAnimation::sequencer(const String& name) const
{
  // go thru all sequencers
  for (SequencerList::const_iterator it = m_sequencers.begin(); it != m_sequencers.end(); ++it)
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
void ImagedAnimation::onSequencerFrameChanged(PSequencer sequencer, s32 frameId)
{
  // update object data
  FrameData& frameData = m_frames[frameId];
  for (List<EGEImagedAnimation::ActionData>::iterator it = frameData.actions.begin(); it != frameData.actions.end(); ++it)
  {
    EGEImagedAnimation::ActionData& action = *it;

    ObjectData& objectData = m_objects.at(action.objectId);

    // update matrix
    objectData.baseFrameMatrix = action.matrix * objectData.baseMatrix;
    
    // apply alignment
    Vector4f translation = objectData.baseFrameMatrix.translation();
    Math::Align(&translation, &m_displaySize, m_baseAlignment, ALIGN_TOP_LEFT);
    objectData.baseFrameMatrix.setTranslation(translation.x, translation.y, translation.z);

    // update priority
    objectData.renderData->setPriority(m_baseRenderPriority + action.queue);
  
    // set clipping rect
   // objectData.renderData->setClipRect(Rectf(translation.x, translation.y, m_displaySize.x, m_displaySize.y));
  }

  // emit
  emit frameChanged(this, frameId);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when sequencer finished animation .*/
void ImagedAnimation::onSequencerFinished(PSequencer sequencer)
{
  // stop
  m_state = STATE_STOPPED;

  // emit
  emit finished(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns current sequencer. */
PSequencer ImagedAnimation::currentSequencer() const 
{ 
  return m_currentSequencer; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
