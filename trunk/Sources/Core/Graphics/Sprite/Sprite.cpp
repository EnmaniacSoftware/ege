#include "Core/Graphics/Sprite/Sprite.h"
#include "Core/Resource/ResourceSpritesheet.h"
#include "Core/Resource/ResourceTextureImage.h"
#include <EGEDebug.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(Sprite)
EGE_DEFINE_DELETE_OPERATORS(Sprite)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Sprite::Sprite(Application* app) : Object(app), 
                                   m_state(STATE_STOPPED), 
                                   m_frameIndex(0)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Sprite::~Sprite()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates object. */
void Sprite::update(const Time& time)
{
  if (isPlaying())
  {
    // update frame time
    m_frameTimeLeft -= time;

    // check if frame time passed
    if (0 >= m_frameTimeLeft.microseconds())
    {
      // go to next frame
      if (++m_frameIndex == static_cast<s32>(m_frameData.size()))
      {
        // we reach end of cycle, repeat
        m_frameIndex = 0;
      }

      // emit
      emit frameChanged(m_frameIndex);

      // add frame time to whats left
      m_frameTimeLeft += m_frameTime;
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Starts playing. */
void Sprite::play()
{
  if (!isPlaying())
  {
    // check if frame time is to be calculated
    if (0 == m_frameTime.microseconds() && (0 < m_frameData.size()))
    {
      m_frameTime = m_duration / static_cast<float32>(m_frameData.size());
      m_frameTimeLeft = m_frameTime;
    }

    // emit first frame straight away
    emit frameChanged(m_frameIndex);

    // change state
    m_state = STATE_PLAYING;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Stops playing. */
void Sprite::stop()
{
  if (isPlaying())
  {
    // reset frame index
    m_frameIndex = 0;

    // change state
    m_state = STATE_STOPPED;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns texture image for current frame. */
PTextureImage Sprite::frameTexture() const
{
  m_textureImage->setRect(m_frameData[m_frameIndex].m_rect);
  return m_textureImage;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets playback duration. */
void Sprite::setDuration(const Time& duration)
{
  m_duration = duration;

  // reset frame time for further evaluation
  m_frameTime = 0.0f;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets frame data. */
void Sprite::setFrameData(const DynamicArray<EGESprite::FrameData>& data)
{
  m_frameData.copy(data);

  // reset frame time for further evaluation
  m_frameTime = 0.0f;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets texture image containing sprite data. */
void Sprite::setTexture(const PTextureImage& texture)
{
  m_textureImage = texture;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
