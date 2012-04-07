#ifndef EGE_CORE_IMAGEDANIMATION_H
#define EGE_CORE_IMAGEDANIMATION_H

/** Imaged animation objects represent discreet instances of image-based animations. These represent set of images being animated (transformed) during the 
 *  lifetime. 
 */

#include <EGE.h>
#include <EGETime.h>
#include <EGEFlags.h>
#include <EGEDynamicArray.h>
#include <EGESignal.h>
#include <EGEImagedAnimation.h>
#include "Core/Graphics/TextureImage.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DECLARE_SMART_CLASS(ImagedAnimation, PImagedAnimation)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class ImagedAnimation : public Object
{
  public:

    ImagedAnimation();
    ImagedAnimation(Application* app, const String& name);
   ~ImagedAnimation();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  signals:
    
    /*! Signal emitted on frame change.
     *  @param sprite     ImagedAnimation object for which frame changed.
     *  @param frameIndex New frame index.
     */
    Signal2<const ImagedAnimation*, s32> frameChanged;
    /*! Signal emitted when playback is finished.
     *  @param sprite     ImagedAnimation object for which playback is finished.
     */
    Signal1<const ImagedAnimation*> finished;

  public:

    /*! Available finish policy. */
    enum FinishPolicy
    {
      FP_STOP,                        /*!< ImagedAnimation stops at finish. */
      FP_REPEAT,                      /*!< ImagedAnimation repeats at finish. */
      FP_PING_PONG                    /*!< ImagedAnimation ping-pongs at finish. */
    };

  public:

    /* Updates object. */
    void update(const Time& time);
    /* Starts playing. */
    void play();
    /* Stops playing. */
    void stop();
    /* Returns texture image for current frame. */
    PTextureImage frameTexture() const;
    /* Sets playback duration. */
    void setDuration(const Time& duration);
    /* Sets frame data. */
    //void setFrameData(const DynamicArray<EGEImagedAnimation::FrameData>& data);
    /* Sets texture image containing sprite data. */
    void setTexture(const PTextureImage& texture);
    /*! Returns name. */
    inline const String& name() const { return m_name; }
    /* Sets name. */
    void setName(const String& name);
    /*! Returns TRUE if sprite is being played. */
    inline bool isPlaying() const { return (state() & STATE_PLAYING) ? true : false; }
    /* Sets/unsets finish policy. */
    void setFinishPolicy(FinishPolicy policy);

  private:

    /*! Internal state flags. */
    enum StateFlags
    {
      STATE_STOPPED = 0x00,               /*!< Playback is stopped. */
      STATE_PLAYING = 0x01                /*!< Playing. */
    };

	  EGE_DECLARE_FLAGS(State, StateFlags)

  private:

    /*! Returns current state. */
    inline State state() const { return m_state; }

  private:

    /*! State flags. */
    State m_state;
    /*! Name. */
    String m_name;
    /*! Current frame index (within m_frames). */
    s32 m_frameIndex;
    /*! Frame duration time. */
    Time m_frameTime;
    /*! Current frame duration left. */
    Time m_frameTimeLeft;
    /*! Playback duration. */
    Time m_duration;
    /*! List of all frames in correct sequence for playback. */
    //DynamicArray<EGEImagedAnimation::FrameData> m_frameData;
    /*! Texture image with sprite pixel data. */
    PTextureImage m_textureImage;
    /*! Finish policy. */
    FinishPolicy m_finishPolicy;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_IMAGEDANIMATION_H