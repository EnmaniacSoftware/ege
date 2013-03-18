#ifndef EGE_CORE_TIMELINE_H
#define EGE_CORE_TIMELINE_H

#include "EGE.h"
#include "EGETime.h"
#include "EGESignal.h"
#include "EGETimeline.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(TimeLine, PTimeLine)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class TimeLine : public Object
{
  public:

    TimeLine(Application* app);
   ~TimeLine();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! Available states. */
    enum State
    {
      STATE_IDLE,
      STATE_RUNNING
    };

  signals:

    /*! Signal emitted when frame changes. 
     *  @param s32 Frame to which change occured. 
     */
    Signal1<s32> frameChanged;
    /*! Signal emitted when time line reaches the end and does not loop. */
    Signal0<> finished;

  public:

    /*! Resets time line. */
    void reset();
    /*! Updates objects. */
    void update(const Time& time);
    /*! Returns current state. */
    State state() const { return m_state; }

    /*! Sets duration. */
    void setDuration(const Time& time);
    /*! Sets start frame. */
    void setStartFrame(s32 frame);
    /*! Sets end frame. */
    void setEndFrame(s32 frame);
    /*! Sets loop count. */
    void setLoopCount(s32 count);

  public slots:

    /*! Starts time line. */
    void start();
    /*! Stops time line. */
    void stop();

  private:

    /*! Returns frame corresponding to given time. */
    s32 frameForTime(const Time& time) const;

  private:

    /*! Duration. */
    Time m_duration;
    /*! Start frame. */
    s32 m_startFrame;
    /*! End frame. */
    s32 m_endFrame;
    /*! Loop count. 0 - infinite loop. */
    s32 m_loopCount;
    /*! Current state. */
    State m_state;
    /*! Direction. */
    EGETimeLine::Direction m_direction;
    /*! Time passes since start. */
    Time m_timePassed;
    /*! Last notified frame. */
    s32 m_lastFrame;
    /*! Current loop. */
    s32 m_loop;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_TIMELINE_H
