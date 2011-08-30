#include "Core/Timer/TimeLine.h"
#include <EGEMath.h>
#include <EGEDebug.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(TimeLine)
EGE_DEFINE_DELETE_OPERATORS(TimeLine)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TimeLine::TimeLine(Application* app) : Object(app), m_duration(1.0f), m_startFrame(0), m_endFrame(0), m_loopCount(1), m_state(STATE_IDLE), 
                                       m_direction(EGETimeLine::DIRECTION_FORWARD)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TimeLine::~TimeLine()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Resets time line. */
void TimeLine::reset()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets duration. */
void TimeLine::setDuration(const Time& time)
{
  if (0 >= time.microseconds())
  {
    EGE_WARNING("TimeLine::setDuration - Invalid value");
    return;
  }

  m_duration = time;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets start frame. */
void TimeLine::setStartFrame(s32 frame)
{
  m_startFrame = frame;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets end frame. */
void TimeLine::setEndFrame(s32 frame)
{
  m_endFrame = frame;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Starts time line. */
void TimeLine::start()
{
  m_timePassed = 0.0f;
  m_lastFrame = m_startFrame;
  m_loop = 0;

  // change state
  m_state = STATE_RUNNING;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Stops time line. */
void TimeLine::stop()
{
  // change state
  m_state = STATE_IDLE;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets loop count. */
void TimeLine::setLoopCount(s32 count)
{
  m_loopCount = count;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates objects. */
void TimeLine::update(const Time& time)
{
  // check if running
  if (STATE_RUNNING == state())
  {
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
/*! Returns frame corresponding to given time. */
s32 TimeLine::frameForTime(const Time& time) const
{
  if (EGETimeLine::DIRECTION_FORWARD == m_direction)
  {
      return m_startFrame + (s32) ((m_endFrame - m_startFrame) * time.seconds() / m_duration.seconds());
  }

  return m_startFrame + (s32) Math::Ceil((m_endFrame - m_startFrame) * time.seconds() / m_duration.seconds());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
