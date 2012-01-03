#ifndef EGE_CORE_SPRITE_H
#define EGE_CORE_SPRITE_H

/** Sprite objects represent discreet instances of sprites. Sprites represent rectangular subset of spritesheets which define their look.
 *  Usually, sprites change their look (area within sheet) while time passes. This can be used to achieve some basic frame based animated images.
 */

#include <EGE.h>
#include <EGETime.h>
#include <EGEFlags.h>
#include <EGEDynamicArray.h>
#include <EGESignal.h>
#include <EGESprite.h>
#include "Core/Graphics/TextureImage.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DECLARE_SMART_CLASS(Sprite, PSprite)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Sprite : public Object
{
  public:

    Sprite();
    Sprite(Application* app, const String& name);
   ~Sprite();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  signals:
    
    /*! Signal emitted on frame change.
     *  @param sprite     Sprite object for which frame changed.
     *  @param frameIndex New frame index.
     */
    Signal2<const Sprite*, s32> frameChanged;
    /*! Signal emitted when playback is finished.
     *  @param sprite     Sprite object for which playback is finished.
     */
    Signal1<const Sprite*> finished;

  public:

    /*! Available finish policy. */
    enum FinishPolicy
    {
      FP_STOP,                        /*!< Sprite stops at finish. */
      FP_REPEAT,                      /*!< Sprite repeats at finish. */
      FP_PING_PONG                    /*!< Sprite ping-pongs at finish. */
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
    void setFrameData(const DynamicArray<EGESprite::FrameData>& data);
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
    DynamicArray<EGESprite::FrameData> m_frameData;
    /*! Texture image with sprite pixel data. */
    PTextureImage m_textureImage;
    /*! Finish policy. */
    FinishPolicy m_finishPolicy;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_SPRITE_H