#include "Core/Sequencer/Sequencer.h"
#include <EGEDebug.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(Sequencer)
EGE_DEFINE_DELETE_OPERATORS(Sequencer)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Sequencer::Sequencer() :  Object(NULL),
                          m_frameIndex(0),
                          m_repeatable(false)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Sequencer::~Sequencer()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IAnimation override. Updates animation. */
void Sequencer::update(const Time& time)
{
  // NOTE: number of transitions equals = number of frames - 1

  // check if multi frame case
  if (m_frameIndex < static_cast<s32>(m_framesIds.size()) - 1)
  {
    m_frameTimeLeft -= time;
    if (0 >= m_frameTimeLeft.microseconds())
    {
      // go to next frame
      ++m_frameIndex;

      // check if end reached
      if (m_frameIndex >= static_cast<s32>(m_framesIds.size()) - 1)
      {
        // check if done
        if (!m_repeatable)
        {
          // make sure we stop at end of one before last frame
          m_frameIndex    = static_cast<s32>(m_framesIds.size() - 1);
          m_frameTimeLeft = m_frameDuration;

          // emit
          emit frameChanged(this, m_framesIds[m_frameIndex]);

          // done
          emit finished(this);
          return;
        }

        // start from begining
        m_frameIndex = 0;
      }

      // emit
      emit frameChanged(this, m_framesIds[m_frameIndex]);

      // update frame time left
      m_frameTimeLeft += m_frameDuration;
    }
  }
  // check if special case single frame sequence
  else if ((0 == m_frameIndex) && (1 == m_framesIds.size()))
  {
    // check if not notified yet
    if (m_frameTimeLeft == m_frameDuration)
    {
      // check if done
      if (!m_repeatable)
      {
        // stop at the begining of only frame
        m_frameIndex    = 0;
        m_frameTimeLeft = 0.0f;

        // emit
        emit frameChanged(this, m_framesIds[m_frameIndex]);

        // done
        emit finished(this);
        return;
      }

      // start from begining
      m_frameIndex = 0;

      // emit
      emit frameChanged(this, m_framesIds[m_frameIndex]);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Starts sequencer. */
void Sequencer::start()
{
  // restart from begining
  m_frameIndex    = 0;
  m_frameTimeLeft = m_frameDuration;

  // emit first frame immediately
  emit frameChanged(this, m_framesIds[m_frameIndex]);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets frame duration. */
void Sequencer::setFrameDuration(const Time& time)
{
  m_frameDuration = time;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets frame data. */
void Sequencer::setFrameData(const IntArray& frameIds)
{
  m_framesIds = frameIds;

  // reset data
  m_frameIndex = 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets repeat flag. */
void Sequencer::setRepeatable(bool set)
{
  m_repeatable = set;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets name. */
void Sequencer::setName(const String& name)
{
  m_name = name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns given frame id. */
s32 Sequencer::frameId(s32 frameIndex) const
{
  s32 index = Math::Bound(frameIndex, 0, static_cast<s32>(m_framesIds.size() - 1));

  return m_framesIds[index];
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns normalized position between current and next frame. */
float32 Sequencer::normalizedFrameTime() const
{
  // sanity check
  if (0 == m_frameDuration.microseconds())
  {
    return 0.0f;
  }

  return 1.0f - m_frameTimeLeft.seconds() / m_frameDuration.seconds();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
