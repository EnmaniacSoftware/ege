#include "Core/Animation/ImagedAnimation.h"
#include "Core/Resource/ResourceImagedAnimation.h"
#include "Core/Resource/ResourceTextureImage.h"
#include <EGEDebug.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ImagedAnimation)
EGE_DEFINE_DELETE_OPERATORS(ImagedAnimation)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ImagedAnimation::ImagedAnimation() : Object(NULL), 
                   m_state(STATE_STOPPED), 
                   m_name(""),
                   m_frameIndex(0),
                   m_finishPolicy(FP_STOP)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ImagedAnimation::ImagedAnimation(Application* app, const String& name) : Object(app), 
                                                       m_state(STATE_STOPPED), 
                                                       m_name(name),
                                                       m_frameIndex(0),
                                                       m_finishPolicy(FP_STOP)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ImagedAnimation::~ImagedAnimation()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates object. */
void ImagedAnimation::update(const Time& time)
{
/*  if (isPlaying())
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
  }*/
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Starts playing. */
void ImagedAnimation::play()
{
 /* if (!isPlaying() && !m_frameData.empty())
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
  }*/
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Stops playing. */
void ImagedAnimation::stop()
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
PTextureImage ImagedAnimation::frameTexture() const
{
  return NULL;
  //m_textureImage->setRect(m_frameData[m_frameIndex].m_rect);
//  return m_textureImage;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets playback duration. */
void ImagedAnimation::setDuration(const Time& duration)
{
  m_duration = duration;

  // reset frame time for further evaluation
  m_frameTime = 0.0f;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets frame data. */
//void ImagedAnimation::setFrameData(const DynamicArray<EGEImagedAnimation::FrameData>& data)
//{
//  m_frameData.copy(data);
//
//  // reset frame time for further evaluation
//  m_frameTime = 0.0f;
//}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets texture image containing sprite data. */
void ImagedAnimation::setTexture(const PTextureImage& texture)
{
  m_textureImage = texture;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets/unsets finish policy. */
void ImagedAnimation::setFinishPolicy(FinishPolicy policy)
{
  m_finishPolicy = policy;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets name. */
void ImagedAnimation::setName(const String& name)
{
  m_name = name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
