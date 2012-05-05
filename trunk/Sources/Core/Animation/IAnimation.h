#ifndef EGE_CORE_ANIMATION_INTERFACE_H
#define EGE_CORE_ANIMATION_INTERFACE_H

/** Interface for all kinds of animations. 
 */

#include <EGEString.h>
#include <EGETime.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class IAnimation
{
  public:

    /*! Starts playback with a given sequencer. 
     *  @param sequencerName  Name of the sequencer to use for playback.
     *  @note If animation for given sequencer is was paused it will be resumed. Otherwise, animation will be started from the begining.
     */
    virtual EGEResult play(const String& sequencerName) = 0;
    /*! Starts playback with a given sequencer. 
     *  @param sequencerIndex Index of the sequencer to use for playback. Negative value replays last sequence if available.
     *  @note If animation for given sequencer was paused it will be resumed. Otherwise, animation will be started from the begining.
     */
    virtual EGEResult play(s32 sequencerIndex) = 0;
    /*! Stops playback. */
    virtual void stop() = 0;
    /*! Pauses playback. */
    virtual void pause() = 0;
    /*! Returns TRUE if animation is being played. */
    virtual bool isPlaying() const = 0;
    /*! Returns TRUE if animation is paused. */
    virtual bool isPaused() const = 0;
    /*! Returns TRUE if animation is stopped. */
    virtual bool isStopped() const = 0;
    /*! Updates animation. */
    virtual void update(const Time& time) = 0;

  public:

    /*! Available states. */
    enum State
    {
      STATE_STOPPED = 0,                /*!< Playback is stopped. */
      STATE_PLAYING,                    /*!< Playing. */
      STATE_PAUSED                      /*!< Playback being paused. */
    };
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_ANIMATION_INTERFACE_H