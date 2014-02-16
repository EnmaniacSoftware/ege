#include "Core/Sequencer/Sequencer.h"
#include "EGEMath.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

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
void Sequencer::update(const Time& time)
{
  // update frame time
  m_frameTimeLeft -= time;
  if (0 >= m_frameTimeLeft.microseconds())
  {
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
    else
    {
      // go to next frame
      ++m_frameIndex;
    }

    // emit
    emit frameChanged(this, m_framesIds[m_frameIndex]);

    // update frame time left
    m_frameTimeLeft += m_frameDuration;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Sequencer::start()
{
  // restart from begining
  m_frameIndex    = 0;
  m_frameTimeLeft = m_frameDuration;

  // emit first frame immediately
  emit frameChanged(this, m_framesIds[m_frameIndex]);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Sequencer::setFrameDuration(const Time& time)
{
  m_frameDuration = time;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Sequencer::setFrameData(const IntArray& frameIds)
{
  m_framesIds = frameIds;

  // reset data
  m_frameIndex = 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Sequencer::setRepeatable(bool set)
{
  m_repeatable = set;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Sequencer::setName(const String& name)
{
  m_name = name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s32 Sequencer::frameId(s32 frameIndex) const
{
  s32 index = Math::Clamp(frameIndex, 0, static_cast<s32>(m_framesIds.size() - 1));

  return m_framesIds[index];
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
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

EGE_NAMESPACE_END