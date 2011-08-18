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

    Sprite(Application* app);
   ~Sprite();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  signals:
    
    Signal1<s32> frameChanged;

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
    /*! Returns TRUE if sprite is being played. */
    inline bool isPlaying() const { return (state() & STATE_PLAYING) ? true : false; }

  private:

    /*! State flags. */
    State m_state;
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
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_SPRITE_H