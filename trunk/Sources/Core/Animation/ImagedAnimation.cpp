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
                                     m_frameCount(0),
                                     m_finishPolicy(FP_STOP)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ImagedAnimation::ImagedAnimation(Application* app, const String& name) : Object(app), 
                                                                         m_state(STATE_STOPPED), 
                                                                         m_name(name),
                                                                         m_frameIndex(0),
                                                                         m_frameCount(0),
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
  if (isPlaying())
  {
    // update frame time
    m_frameTimeLeft -= time;

    // check if frame time passed
    if (0 >= m_frameTimeLeft.microseconds())
    {
      // go to next frame
      if (++m_frameIndex == static_cast<s32>(m_frameCount))
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
void ImagedAnimation::play()
{
  if (!isPlaying() && !m_objects.empty())
  {
    // reset frame
    m_frameIndex = 0;

    // recalculate frame time 
    m_frameTime     = m_duration / static_cast<float32>(m_frameCount);
    m_frameTimeLeft = m_frameTime;

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
 *  @param material    Material used by the object.
 *  @param baseMatrix  Object's base transformation matrix.
 *  @param actions     Array of animation actions of the object. Each entry corresponds to a given frame.
 */
EGEResult ImagedAnimation::addObject(s32 objectId, PMaterial material, const Matrix4f& baseMatrix, const DynamicArray<EGEImagedAnimation::ActionData>& actions)
{
  ObjectData data;

  // check if object already in
  if (m_objects.contains(objectId))
  {
    // error!
    return EGE_ERROR_ALREADY_EXISTS;
  }

  // create object
  data.matrix     = baseMatrix;
  data.renderData = RenderObjectFactory::CreateQuadXY(app(), String::Format("image-animation-object-%d", m_objects.size()), Vector4f::ZERO, Vector2f::ONE,
                                                      ALIGN_TOP_LEFT, EGEVertexBuffer::ST_V2_T2, 50, EGEGraphics::RPT_TRIANGLE_STRIPS, 
                                                      EGEVertexBuffer::UT_DYNAMIC_WRITE_DONT_CARE);
  data.actions    = actions;

  // check if properly created
  if ((NULL != data.renderData))
  {
    // set material
    data.renderData->setMaterial(material);

    // cache number of frames
    m_frameCount = Math::Max(m_frameCount, static_cast<s32>(actions.size()));

    // add into pool
    m_objects.insert(objectId, data);
    return EGE_SUCCESS;
  }

  return EGE_ERROR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Renders animation. */
void ImagedAnimation::addForRendering(Renderer* renderer, const Matrix4f& transform)
{
  for (ObjectDataMap::const_iterator it = m_objects.begin(); it != m_objects.end(); ++it)
  {
    const ObjectData& data = it->second;

    s32 frameIndex = Math::Min(m_frameIndex, static_cast<s32>(data.actions.size() - 1));

    Matrix4f mat = transform * data.matrix * data.actions[frameIndex].matrix;
    Matrix4f mat2 = data.actions[frameIndex].matrix * data.matrix * transform;
    Matrix4f mat3 = data.matrix * data.actions[frameIndex].matrix * transform;

    if (data.actions.size() > 1)
    {
      int a = 1;
    }

    data.renderData->setPriority(50 + data.actions[frameIndex].queue);
    renderer->addForRendering(data.renderData, mat3);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates objects for current frame index. */
void ImagedAnimation::updateObjects()
{
  //for (ObjectDataMap::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
  //{
  //  ObjectData& data = it->second;

  //  data.matrix = data.actions[m_frameIndex].matrix;
  //}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
