#include "Core/Graphics/Sprite/Sprite.h"
#include "Core/Resource/ResourceSpritesheet.h"
#include "Core/Resource/ResourceTextureImage.h"
#include <EGEDebug.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(Sprite)
EGE_DEFINE_DELETE_OPERATORS(Sprite)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Sprite::Sprite(Application* app, const String& name) : Object(app), 
                                                       m_state(STATE_STOPPED), 
                                                       m_name(name),
                                                       m_frameIndex(0),
                                                       m_finishPolicy(FP_STOP)
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
        // we reach end of cycle

        // process according to policy
        switch (m_finishPolicy)
        {
          case FP_REPEAT:

            m_frameIndex = 0;
            break;

          case FP_PING_PONG:

            EGE_ASSERT(false && "Implement!");
            break;

          case FP_STOP:

            // change state
            m_state = STATE_STOPPED;

            // set current frame to last frame
            m_frameIndex--;

            // emit
            emit finished(this);

            // done
            return;

          default:

            EGE_ASSERT(false && "Unsupported finish policy!");
            break;
        }
      }

      // emit
      emit frameChanged(this, m_frameIndex);

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
    // reset frame
    m_frameIndex = 0;

    // check if frame time is to be calculated
    if (0 == m_frameTime.microseconds() && (0 < m_frameData.size()))
    {
      m_frameTime = m_duration / static_cast<float32>(m_frameData.size());
      m_frameTimeLeft = m_frameTime;
    }

    // emit first frame straight away
    emit frameChanged(this, m_frameIndex);

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
/*! Sets/unsets finish policy. */
void Sprite::setFinishPolicy(FinishPolicy policy)
{
  m_finishPolicy = policy;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
