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
  if (m_frameIndex < static_cast<s32>(m_framesIds.size()))
  {
    m_frameTimeLeft -= time;
    if (0 >= m_frameTimeLeft.microseconds())
    {
      // go to next frame
      ++m_frameIndex;
      if (m_frameIndex >= static_cast<s32>(m_framesIds.size()))
      {
        // check if done
        if (!m_repeatable)
        {
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
/*! Returns current frame id. */
s32 Sequencer::currentFrameId() const
{
  s32 index = Math::Bound(m_frameIndex, 0, static_cast<s32>(m_framesIds.size() - 1));

  return m_framesIds[index];
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
