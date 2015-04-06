#ifndef EGE_CORE_ANIMATION_KEYFRAMEANIMATION_H
#define EGE_CORE_ANIMATION_KEYFRAMEANIMATION_H

/** Base class for all kinds of key-frame animations. 
 */

#include "EGEString.h"
#include "EGETime.h"
#include "EGEDynamicArray.h"
#include "EGESignal.h"
#include "EGESequencer.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Engine;

EGE_PREDECLARE_SMART_CLASS(KeyFrameAnimation, PKeyFrameAnimation)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class KeyFrameAnimation : public Object
{
  public:

    KeyFrameAnimation(Engine& engine, const String& name);
    virtual ~KeyFrameAnimation();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public signals:
    
    /*! Signal emitted on frame change.
     *  @param frameIndex New frame index.
     */
    Signal1<s32> signalFrameChanged;

    /*! Signal emitted when playback is finished. */
    Signal0<> signalFinished;
    /*! Signal emitted when playback is finished. 
     *  @param animation  Animation which is finished.
     */
    Signal1<PKeyFrameAnimation> signalFinishedWithSelf;

  public:

    /*! Available states. */
    enum State
    {
      EStateStopped = 0,                /*!< Playback is stopped. */
      EStatePlaying,                    /*!< Playing. */
      EStatePaused                      /*!< Playback is being paused. */
    };

  public:

    /*! Starts playback with a sequencer having given name. 
     *  @param sequencerName  Name of the sequencer to use for playback.
     *  @return EGE_SUCCESS if playback has been started. EGE_ERROR_NOT_FOUND is returned if appropriate sequencer could not be found.
     *  @note If animation for the given sequencer was paused it will be resumed. Otherwise, animation will be started from the begining.
     */
    virtual EGEResult play(const String& sequencerName);
    /*! Starts playback with a given sequencer. 
     *  @param sequencerIndex Index of the sequencer to use for playback. Negative value replays last sequence if available.
     *  @return EGE_SUCCESS if playback has been started. EGE_ERROR_NOT_FOUND is returned if appropriate sequencer could not be found.
     *  @note If animation for the given sequencer was paused it will be resumed. Otherwise, animation will be started from the begining.
     */
    virtual EGEResult play(s32 sequencerIndex = 0);
    /*! Returns TRUE if animation is being played. */
    virtual bool isPlaying() const;
    
    /*! Stops playback. */
    virtual void stop();
    /*! Returns TRUE if animation is stopped. */
    virtual bool isStopped() const;

    /*! Pauses playback. */
    virtual void pause();
    /*! Returns TRUE if animation is paused. */
    virtual bool isPaused() const;

    /*! Updates animation. 
     *  @param  time  Time increment.
     */
    virtual void update(const Time& time);

    /*! Sets FPS playback value. 
     *  @param  value Nmber of keys animated per second.
     */
    void setKeysPerSecond(float32 value);

    /*! Adds sequencer. 
     *  @param  sequencer Sequencer object to add.
     *  @return EGE_SUCCESS if sequencer has been added. EGE_ERROR_ALREADY_EXISTS if sequencer with given name already exists.
     *          EGE_ERROR in any other case.
     */
    EGEResult addSequencer(const PSequencer& sequencer);

    /*! Returns name. */
    const String& name() const;
    /*! Sets name. 
     *  @param name Animation name to set.
     */
    void setName(const String& name);

  protected:

    /*! Returns engine object. */
    Engine& engine() const;
    /*! Returns current state. */
    State state() const;

    /*! Returns sequencer of a given name. 
     *  @param  name  Name of the sequencer to find.
     *  @return Sequencer object found. NULL if no sequencer with given name has been found.
     */
    PSequencer sequencer(const String& name) const;
    /*! Returns current sequencer. */
    PSequencer currentSequencer() const;

  protected slots:

    /*! Slot called when sequencer animated into new frame. */
    virtual void onSequencerFrameChanged(s32 frameId);
    /*! Slot called when sequencer finished animation .*/
    virtual void onSequencerFinished();

  protected:

    typedef DynamicArray<PSequencer> SequencerArray;

  protected:

    /*! Reference to engine. */
    Engine& m_engine;
    /*! Current state. */
    State m_state;
    /*! Name. */
    String m_name;
    /*! Frame duration time. */
    Time m_frameDuration;
    /*! Array of all sequencers. */
    SequencerArray m_sequencers;
    /*! Current sequencer. Can be NULL. */
    PSequencer m_currentSequencer;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_ANIMATION_KEYFRAMEANIMATION_H