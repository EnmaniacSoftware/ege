#ifndef EGE_CORE_SPRITEANIMATION_H
#define EGE_CORE_SPRITEANIMATION_H

/** SpriteAnimation objects represent discreet instances of sprites. Sprites represent rectangular subset of spritesheets which define their look.
 *  Usually, sprites change their look (area within sheet) while time passes. This can be used to achieve some basic frame based animated images.
 */

#include <EGE.h>
#include <EGETime.h>
#include <EGEFlags.h>
#include <EGEDynamicArray.h>
#include <EGESignal.h>
#include <EGESpriteAnimation.h>
#include <EGEAnimation.h>
#include <EGESequencer.h>
#include "Core/Graphics/TextureImage.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(SpriteAnimation, PSpriteAnimation)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class SpriteAnimation : public Object, public IAnimation
{
  public:

    SpriteAnimation();
    SpriteAnimation(Application* app, const String& name);
   ~SpriteAnimation();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  signals:
    
    /*! Signal emitted on frame change.
     *  @param sprite     SpriteAnimation object for which frame changed.
     *  @param frameIndex New frame index.
     */
    Signal2<PSpriteAnimation, s32> frameChanged;
    /*! Signal emitted when playback is finished.
     *  @param sprite     SpriteAnimation object for which playback is finished.
     */
    Signal1<PSpriteAnimation> finished;

  public:

    /* IAnimation override. Starts playback with a given sequencer. 
     * @param sequencerName  Name of the sequencer to use for playback.
     * @note If animation for given sequencer is was paused it will be resumed. Otherwise, animation will be started from the begining.
     */
    EGEResult play(const String& sequencerName) override;
    /* IAnimation override. Starts playback with a given sequencer. 
     * @param sequencerIndex Index of the sequencer to use for playback. Negative value replays last sequence if available.
     * @note If animation for given sequencer is was paused it will be resumed. Otherwise, animation will be started from the begining.
     */
    EGEResult play(s32 sequencerIndex = 0) override;
    /* IAnimation override. Stops playback. */
    void stop() override;
    /* IAnimation override. Pauses playback. */
    void pause() override;
    /* IAnimation override. Returns TRUE if animation is being played. */
    bool isPlaying() const override;
    /* IAnimation override. Returns TRUE if animation is paused. */
    bool isPaused() const override;
    /* IAnimation override. Returns TRUE if animation is stopped. */
    bool isStopped() const override;
    /* IAnimation override. Updates animation. */
    void update(const Time& time) override;

    /* Returns texture image for current frame. */
    PTextureImage frameTexture() const;
    /* Sets FPS playback value. */
    void setFPS(float32 fps);
    /* Sets frame data. */
    void setFrameData(const DynamicArray<EGESprite::FrameData>& data);
    /* Sets texture image containing sprite data. */
    void setTexture(const PTextureImage& texture);
    /*! Returns name. */
    inline const String& name() const { return m_name; }
    /* Sets name. */
    void setName(const String& name);

    /* Adds sequencer. */
    void addSequencer(const PSequencer& sequencer);
    /* Returns current sequencer. */
    PSequencer currentSequencer() const;

  private:

    /*! Returns current state. */
    inline State state() const { return m_state; }
    /* Returns sequencer of a given name. */
    PSequencer sequencer(const String& name) const;

  private slots:

    /* Slot called when sequencer animated into new frame. */
    void onSequencerFrameChanged(PSequencer sequencer, s32 frameId);
    /* Slot called when sequencer finished animation .*/
    void onSequencerFinished(PSequencer sequencer);

  private:

    typedef DynamicArray<PSequencer> SequencerArray;

  private:

    /*! Current state. */
    State m_state;
    /*! Name. */
    String m_name;
    /*! Frame duration time. */
    Time m_frameDuration;
    /*! List of all frames in correct sequence for playback. */
    DynamicArray<EGESprite::FrameData> m_frameData;
    /*! Texture image with sprite pixel data. */
    PTextureImage m_textureImage;
    /*! Array of all sequencers. */
    SequencerArray m_sequencers;
    /*! Current sequencer. */
    PSequencer m_currentSequencer;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_SPRITEANIMATION_H