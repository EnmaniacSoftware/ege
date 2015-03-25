#include "Core/Animation/Interface/Sequencer.h"
#include "EGEMath.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(Sequencer)
EGE_DEFINE_DELETE_OPERATORS(Sequencer)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Sequencer::Sequencer() 
: Object()
, m_frameIndex(0)
, m_repeatable(false)
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
    if (m_frameIndex >= m_framesIds.size() - 1)
    {
      // check if done
      if (!m_repeatable)
      {
        // make sure we stop at end of one before last frame
        m_frameIndex    = m_framesIds.size() - 1;
        m_frameTimeLeft = m_frameDuration;

        // emit
        emit signalFrameChanged(m_framesIds[m_frameIndex]);

        // done
        emit signalFinished();
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
    emit signalFrameChanged(m_framesIds[m_frameIndex]);

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
  emit signalFrameChanged(m_framesIds[m_frameIndex]);
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
  s32 index = Math::Clamp(frameIndex, 0, m_framesIds.size() - 1);

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
s32 Sequencer::currentFrameIndex() const 
{ 
  return m_frameIndex; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s32 Sequencer::frameCount() const 
{ 
  return m_framesIds.size(); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const String& Sequencer::name() const 
{ 
  return m_name; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END