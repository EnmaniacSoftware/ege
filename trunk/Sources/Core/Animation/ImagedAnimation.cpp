#include "Core/Animation/ImagedAnimation.h"
#include "Core/Resource/ResourceImagedAnimation.h"
#include "Core/Resource/ResourceTextureImage.h"
#include <EGEPhysics.h>
#include <EGEGraphics.h>
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
                                     m_finishPolicy(FP_STOP),
                                     m_baseRenderPriority(EGEGraphics::RP_MAIN)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ImagedAnimation::ImagedAnimation(Application* app, const String& name) : Object(app), 
                                                                         m_state(STATE_STOPPED), 
                                                                         m_name(name),
                                                                         m_frameIndex(0),
                                                                         m_finishPolicy(FP_STOP),
                                                                         m_baseRenderPriority(EGEGraphics::RP_MAIN)
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
  if (isPlaying())
  {
    // update frame time
    m_frameTimeLeft -= time;

    // check if frame time passed
    if (0 >= m_frameTimeLeft.microseconds())
    {
      // go to next frame
      if (++m_frameIndex == static_cast<s32>(m_frames.size()))
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

      // update objects
      update(m_frameIndex);

      // emit
      emit frameChanged(this, m_frameIndex);

      // add frame time to whats left
      m_frameTimeLeft += m_frameTime;
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Starts playing. */
void ImagedAnimation::play()
{
  if (!isPlaying() && !m_objects.empty() && !m_frames.empty())
  {
    // reset frame
    m_frameIndex = 0;

    // recalculate frame time 
    m_frameTime     = m_duration / static_cast<float32>(m_frames.size());
    m_frameTimeLeft = m_frameTime;

    // update objects
    update(m_frameIndex);

    // emit first frame straight away
    emit frameChanged(this, m_frameIndex);

    // change state
    m_state = STATE_PLAYING;
  }
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
/*! Sets playback duration. */
void ImagedAnimation::setDuration(const Time& duration)
{
  m_duration = duration;

  // reset frame time for further evaluation
  m_frameTime = 0.0f;
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
/*! Adds object with a given id to animation. 
 *  @param objectId    Object Id which is being added.
 *  @param size        Object size (in pixels).
 *  @param material    Material used by the object.
 *  @param baseMatrix  Object's base transformation matrix.
 */
EGEResult ImagedAnimation::addObject(s32 objectId, const Vector2f& size, PMaterial material, const Matrix4f& baseMatrix)
{
  ObjectData data;

  // check if object already in
  if (m_objects.contains(objectId))
  {
    // error!
    return EGE_ERROR_ALREADY_EXISTS;
  }

  // create object
  data.baseFrameMatrix  = Matrix4f::IDENTITY;
  data.baseMatrix       = baseMatrix;
  data.renderData       = RenderObjectFactory::CreateQuadXY(app(), String::Format("image-animation-object-%d", m_objects.size()), Vector4f::ZERO, size,
                                                            ALIGN_TOP_LEFT, EGEVertexBuffer::ST_V2_T2, 0, EGEGraphics::RPT_TRIANGLE_STRIPS, 
                                                            EGEVertexBuffer::UT_DYNAMIC_WRITE_DONT_CARE);

  // check if properly created
  if ((NULL != data.renderData))
  {
    // set material
    data.renderData->setMaterial(material);

    // add into pool
    m_objects.insert(objectId, data);
    return EGE_SUCCESS;
  }

  return EGE_ERROR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Adds frame data.
 *  @param action  List of action to be processed at given frame.
 *  @note  This creates new frame and appends it into existing ones.
 */
EGEResult ImagedAnimation::addFrameData(const List<EGEImagedAnimation:: ActionData>& actions)
{
  FrameData frameData;
  frameData.actions = actions;

  m_frames.push_back(frameData);

  // if first frame added, update object
  if (1 == m_frames.size())
  {
    update(0);
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Renders animation. */
void ImagedAnimation::addForRendering(Renderer* renderer, const Matrix4f& transform)
{
  // NOTE: it is imperative to render objects in the order of appearence for a given frame

  const FrameData& frameData = m_frames[m_frameIndex];
  for (List<EGEImagedAnimation::ActionData>::const_iterator it = frameData.actions.begin(); it != frameData.actions.end(); ++it)
  {
    const EGEImagedAnimation::ActionData& action = *it;

    const ObjectData& objectData = m_objects.at(action.objectId);
    renderer->addForRendering(objectData.renderData, transform * objectData.baseFrameMatrix);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets base render priority. */
void ImagedAnimation::setBaseRenderPriority(EGEGraphics::RenderPriority basePriority)
{
  m_baseRenderPriority = basePriority;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates animation objects with given frame data. */
void ImagedAnimation::update(s32 frameIndex)
{
  FrameData& frameData = m_frames[m_frameIndex];
  for (List<EGEImagedAnimation::ActionData>::iterator it = frameData.actions.begin(); it != frameData.actions.end(); ++it)
  {
    EGEImagedAnimation::ActionData& action = *it;

    ObjectData& objectData = m_objects.at(action.objectId);

    // update matrix
    objectData.baseFrameMatrix = action.matrix * objectData.baseMatrix;

    // update priority
    objectData.renderData->setPriority(m_baseRenderPriority + action.queue);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
