#include "Core/Animation/Interface/KeyFrameAnimation.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const char* KKeyFrameAnimationDebugName = "EGEKeyFrameAnimation";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(KeyFrameAnimation)
EGE_DEFINE_DELETE_OPERATORS(KeyFrameAnimation)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
KeyFrameAnimation::KeyFrameAnimation(Engine& engine, const String& name) : Object()
, m_engine(engine)
, m_name(name)
, m_state(EStateStopped)
{}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
KeyFrameAnimation::~KeyFrameAnimation()
{}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult KeyFrameAnimation::addSequencer(const PSequencer& sequencer)
{
  EGEResult result = EGE_SUCCESS;

  // check if sequencer with such name exists
  if (NULL != this->sequencer(sequencer->name()))
  {
    // cannot add
    result = EGE_ERROR_ALREADY_EXISTS;
  }
  else
  {
    // add to pool
    m_sequencers.append(sequencer);
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PSequencer KeyFrameAnimation::sequencer(const String& name) const
{
  // go thru all sequencers
  for (SequencerArray::ConstIterator it = m_sequencers.begin(); it != m_sequencers.end(); ++it)
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
KeyFrameAnimation::State KeyFrameAnimation::state() const
{
  return m_state;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult KeyFrameAnimation::play(const String& sequencerName)
{
  EGEResult result = EGE_ERROR;

  // check if playback can be resumed
  if (isPaused())
  {
    // check if resuming the same sequencer
    if ((NULL != m_currentSequencer) && (sequencerName == m_currentSequencer->name()))
    {
      // change state
      m_state = EStatePlaying;
    }
    else
    {
      // stop it so it can be restarted from the begining
      stop();
    }
  }

  // check if still not playing
  if (!isPlaying())
  {
    // get sequencer
    m_currentSequencer = this->sequencer(sequencerName);
    if (NULL == m_currentSequencer)
    {
      // error!
      result = EGE_ERROR_NOT_FOUND;
    }
    else
    {
      // setup sequencer
      m_currentSequencer->setFrameDuration(m_frameDuration);

      // connect to sequencer
      ege_connect(m_currentSequencer, signalFrameChanged, this, KeyFrameAnimation::onSequencerFrameChanged);
      ege_connect(m_currentSequencer, signalFinished, this, KeyFrameAnimation::onSequencerFinished);

      // change state
      m_state = EStatePlaying;

      // start sequencer
      m_currentSequencer->start();

      result = EGE_SUCCESS;
    }
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult KeyFrameAnimation::play(s32 sequencerIndex)
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
void KeyFrameAnimation::stop()
{
  if (isPlaying())
  {
    // change state
    m_state = EStateStopped;
  }

  // disconnect from sequencer
  ege_disconnect(m_currentSequencer, signalFrameChanged, this, KeyFrameAnimation::onSequencerFrameChanged);
  ege_disconnect(m_currentSequencer, signalFinished, this, KeyFrameAnimation::onSequencerFinished);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void KeyFrameAnimation::pause()
{
  // change state
  m_state = EStatePaused;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool KeyFrameAnimation::isPlaying() const
{
  return (EStatePlaying == m_state);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool KeyFrameAnimation::isPaused() const
{
  return (EStatePaused == m_state);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool KeyFrameAnimation::isStopped() const
{
  return (EStateStopped == m_state);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void KeyFrameAnimation::update(const Time& time)
{
  if (isPlaying())
  {
    // NOTE: keep up reference to sequencer here in case animation gets cleared (and all sequencers removed) due to signal emission. If not done here,
    //       this can lead to being emitting while signal has been deallocated due to removal of last sequencer instance from animation
    PSequencer sequencer = m_currentSequencer;
    sequencer->update(time);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void KeyFrameAnimation::onSequencerFrameChanged(s32 frameId)
{
  // emit
  emit signalFrameChanged(frameId);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void KeyFrameAnimation::onSequencerFinished()
{
  // stop
  stop();

  // emit
  emit signalFinished();
  emit signalFinishedWithSelf(*this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void KeyFrameAnimation::setKeysPerSecond(float32 value)
{
  m_frameDuration = (0.0f < value) ? (1.0f / value) : 0.0f;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void KeyFrameAnimation::setName(const String& name)
{
  m_name = name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const String& KeyFrameAnimation::name() const
{
  return m_name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PSequencer KeyFrameAnimation::currentSequencer() const
{
  return m_currentSequencer;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Engine& KeyFrameAnimation::engine() const
{
  return m_engine;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END