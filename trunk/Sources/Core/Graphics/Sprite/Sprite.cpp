#include "Core/Graphics/Sprite/Sprite.h"
#include "Core/Resource/ResourceSpritesheet.h"
#include "Core/Resource/ResourceTextureImage.h"
#include <EGEDebug.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(Sprite)
EGE_DEFINE_DELETE_OPERATORS(Sprite)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Sprite::Sprite() : Object(NULL), 
                   m_state(STATE_STOPPED), 
                   m_name(""),
                   m_repeatDelay(0LL),
                   m_repeatDelayLeft(0LL)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Sprite::Sprite(Application* app, const String& name) : Object(app), 
                                                       m_state(STATE_STOPPED), 
                                                       m_name(name),
                                                       m_repeatDelay(0LL),
                                                       m_repeatDelayLeft(0LL)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Sprite::~Sprite()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IAnimation override. Starts playback with a given sequencer. 
 *  @param sequencerName  Name of the sequencer to use for playback.
 *  @note If animation for given sequencer is was paused it will be resumed. Otherwise, animation will be started from the begining.
 */
EGEResult Sprite::play(const String& sequencerName)
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
    ege_connect(m_currentSequencer, frameChanged, this, Sprite::onSequencerFrameChanged);
    ege_connect(m_currentSequencer, finished, this, Sprite::onSequencerFinished);

    // change state
    m_state = STATE_PLAYING;

    // start sequencer
    m_currentSequencer->start();
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IAnimation override. Starts playback with a given sequencer. 
 *  @param sequencerIndex Index of the sequencer to use for playback.
 *  @note If animation for given sequencer is was paused it will be resumed. Otherwise, animation will be started from the begining.
 */
EGEResult Sprite::play(s32 sequencerIndex)
{
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
void Sprite::stop()
{
  if (isPlaying())
  {
    // disconnect from sequencer
    ege_disconnect(m_currentSequencer, frameChanged, this, Sprite::onSequencerFrameChanged);
    ege_disconnect(m_currentSequencer, finished, this, Sprite::onSequencerFinished);

    // change state
    m_state = STATE_STOPPED;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* IAnimation override. Pauses playback. */
void Sprite::pause()
{
  // change state
  m_state = STATE_PAUSED;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IAnimation override. Returns TRUE if animation is being played. */
bool Sprite::isPlaying() const
{
  return STATE_PLAYING == m_state;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IAnimation override. Returns TRUE if animation is paused. */
bool Sprite::isPaused() const
{
  return STATE_PAUSED == m_state;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IAnimation override. Returns TRUE if animation is stopped. */
bool Sprite::isStopped() const
{
  return STATE_STOPPED == m_state;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IAnimation override. Updates animation. */
void Sprite::update(const Time& time)
{
  if (isPlaying())
  {
    m_currentSequencer->update(time);
  }
}




/*! Updates object. */
/*
void Sprite::update(const Time& time)
{
  if (isPlaying())
  {
    // check if in delay between repeats
    if (0 < m_repeatDelayLeft.microseconds())
    {
      m_repeatDelayLeft -= time;
      if (0 >= m_repeatDelayLeft.microseconds())
      {
        m_frameIndex = 0;

        // emit
        emit frameChanged(this, m_frameIndex);

        // restart frame time
        m_frameTimeLeft = m_frameTime;

        // done
      }
    }
    else
    {
      // update frame time
      m_frameTimeLeft -= time;

      // check if frame time passed
      if (0 >= m_frameTimeLeft.microseconds())
      {
        // go to next frame
        if (++m_frameIndex == static_cast<s32>(m_frameData.size()))
        {
          // we reach end of cycle

          // process according to policy
          switch (m_finishPolicy)
          {
            case FP_REPEAT:

              // check if any restart delay is needed
              if (0 < m_repeatDelay.microseconds())
              {
                // lets wait
                m_repeatDelayLeft = m_repeatDelay;
                return;
              }

              // restart immediately
              m_frameIndex = 0;
              break;

            case FP_PING_PONG:

              EGE_ASSERT(false && "Implement!");
              break;

            case FP_STOP:

              // change state
              m_state = STATE_STOPPED;

              // set current frame to last frame
              m_frameIndex--;

              // emit
              emit finished(this);

              // done
              return;

            default:

              EGE_ASSERT(false && "Unsupported finish policy!");
              break;
          }
        }

        // emit
        emit frameChanged(this, m_frameIndex);

        // add frame time to whats left
        m_frameTimeLeft += m_frameTime;
      }
    }
  }
}*/
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Starts playing. */
//void Sprite::play()
//{
//  if (!isPlaying() && !m_frameData.empty())
//  {
//    // reset data
//    m_frameIndex = 0;
//    m_repeatDelayLeft = 0LL;
//
//    // check if frame time is to be calculated
//    if (0 == m_frameTime.microseconds() && (0 < m_frameData.size()))
//    {
//      m_frameTime = m_duration / static_cast<float32>(m_frameData.size());
//      m_frameTimeLeft = m_frameTime;
//    }
//
//    // emit first frame straight away
//    emit frameChanged(this, m_frameIndex);
//
//    // change state
//    m_state = STATE_PLAYING;
//  }
//}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Stops playing. */
//void Sprite::stop()
//{
//  if (isPlaying())
//  {
//    // reset data
//    m_frameIndex = 0;
//    m_repeatDelayLeft = 0LL;
//
//    // change state
//    m_state = STATE_STOPPED;
//  }
//}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns texture image for current frame. */
PTextureImage Sprite::frameTexture() const
{
  EGE_ASSERT(NULL != m_currentSequencer);

  m_textureImage->setRect(m_frameData[m_currentSequencer->currentFrameId()].m_rect);
  return m_textureImage;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets FPS playback value. */
void Sprite::setFPS(float32 fps)
{
  m_frameDuration = (0.0f < fps) ? (1.0f / fps) : 0.0f;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets frame data. */
void Sprite::setFrameData(const DynamicArray<EGESprite::FrameData>& data)
{
  m_frameData.copy(data);

  // reset frame time for further evaluation
 // m_frameTime = 0.0f;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets texture image containing sprite data. */
void Sprite::setTexture(const PTextureImage& texture)
{
  m_textureImage = texture;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets name. */
void Sprite::setName(const String& name)
{
  m_name = name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets repeat delay. */
void Sprite::setRepeatDelay(const Time& time)
{
  m_repeatDelay = time;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Adds sequencer. */
void Sprite::addSequencer(const PSequencer& sequencer)
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
PSequencer Sprite::sequencer(const String& name) const
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
void Sprite::onSequencerFrameChanged(PSequencer sequencer, s32 frameId)
{
  // update object data
/*  FrameData& frameData = m_frames[frameId];
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
  }*/

  // emit
  emit frameChanged(this, frameId);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when sequencer finished animation .*/
void Sprite::onSequencerFinished(PSequencer sequencer)
{
  // stop
  m_state = STATE_STOPPED;

  // emit
  emit finished(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns current sequencer. */
PSequencer Sprite::currentSequencer() const 
{ 
  return m_currentSequencer; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
