#ifndef EGE_CORE_TIME_TIMELINE_H
#define EGE_CORE_TIME_TIMELINE_H

#include "EGE.h"
#include "EGETime.h"
#include "EGESignal.h"
#include "EGETimeline.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Available direction values. */
enum TimeLineDirection
{
  TLD_FORWARD,              /*!< Forward direction. */
  TLD_BACKWARD,             /*!< Backward direction. */ 
};
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

  public signals:

    /*! Signal emitted when frame changes. 
     *  @param  Frame to which change occured. 
     */
    Signal1<s32> frameChanged;
    /*! Signal emitted when time line reaches the end and does not loop. */
    Signal0<> finished;

  public:

    /*! Updates objects. 
     *  @param  time  Time increment by which update object.
     */
    void update(const Time& time);
    /*! Resets time line. */
    void reset();
    /*! Returns current state. */
    State state() const;

    /*! Sets duration. 
     *  @param  time  Time line duration to set.
     */
    void setDuration(const Time& time);
    /*! Sets start frame. 
     *  @param  frame Start frame index.
     */
    void setStartFrame(s32 frame);
    /*! Sets end frame. 
     *  @param  frame End frame index.
     */
    void setEndFrame(s32 frame);
    /*! Sets loop count. 
     *  @param  count Number of loop to perform.
     */
    void setLoopCount(s32 count);

  public slots:

    /*! Starts time line. */
    void start();
    /*! Stops time line. */
    void stop();

  private:

    /*! Returns frame corresponding to given time. 
     *  @param  time  Time for which calculation is to be performed.
     */
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
    TimeLineDirection m_direction;
    /*! Time passes since start. */
    Time m_timePassed;
    /*! Last notified frame. */
    s32 m_lastFrame;
    /*! Current loop. */
    s32 m_loop;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_TIME_TIMELINE_H
