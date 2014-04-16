#include "Core/Timer/Interface/TimeLine.h"
#include "EGEMath.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(TimeLine)
EGE_DEFINE_DELETE_OPERATORS(TimeLine)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TimeLine::TimeLine(Application* app) : Object(app) 
                                     , m_duration(1.0f) 
                                     , m_startFrame(0) 
                                     , m_endFrame(0)
                                     , m_loopCount(1) 
                                     , m_state(STATE_IDLE) 
                                     , m_direction(TLD_FORWARD)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TimeLine::~TimeLine()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void TimeLine::reset()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void TimeLine::setDuration(const Time& time)
{
  if (0 >= time.microseconds())
  {
    return;
  }

  m_duration = time;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void TimeLine::setStartFrame(s32 frame)
{
  m_startFrame = frame;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void TimeLine::setEndFrame(s32 frame)
{
  m_endFrame = frame;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void TimeLine::start()
{
  m_timePassed = 0.0f;
  m_lastFrame = m_startFrame;
  m_loop = 0;

  // change state
  m_state = STATE_RUNNING;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void TimeLine::stop()
{
  // change state
  m_state = STATE_IDLE;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void TimeLine::setLoopCount(s32 count)
{
  m_loopCount = count;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void TimeLine::update(const Time& time)
{
  // check if running
  if (STATE_RUNNING == state())
  {
    // update time
    m_timePassed += time;

    // get frame for current time
    s32 frame = frameForTime(m_timePassed);

    // check if frame changed
    if (frame != m_lastFrame)
    {
      // notify
      emit frameChanged(frame);

      // store frame
      m_lastFrame = frame;
    }

    // check if end reached
    bool endReached = (m_timePassed >= m_duration);
    if (endReached)
    {
      if ((0 < m_loopCount) && (m_loopCount == ++m_loop))
      {
        // notify
        emit finished();

        // we are done
        m_state = STATE_IDLE;
      }
      else
      {
        // loop from begining
        m_timePassed = 0.0f;
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s32 TimeLine::frameForTime(const Time& time) const
{
  if (TLD_FORWARD == m_direction)
  {
    return m_startFrame + static_cast<s32>((m_endFrame - m_startFrame) * time.seconds() / m_duration.seconds());
  }

  return m_startFrame + static_cast<s32>(Math::Ceil((m_endFrame - m_startFrame) * time.seconds() / m_duration.seconds()));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TimeLine::State TimeLine::state() const 
{ 
  return m_state; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
