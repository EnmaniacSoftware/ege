#include "Core/Graphics/ImagedAnimation/ImagedAnimation.h"
#include "Core/Resource/ResourceImagedAnimation.h"
#include "Core/Resource/ResourceTextureImage.h"
#include <EGESequencer.h>
#include <EGEPhysics.h>
#include <EGEGraphics.h>
#include <EGEDebug.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ImagedAnimation)
EGE_DEFINE_DELETE_OPERATORS(ImagedAnimation)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ImagedAnimation::ImagedAnimation() : Object(NULL), 
                                     m_state(STATE_STOPPED), 
                                     m_name(""),
                                     m_baseRenderPriority(EGEGraphics::RP_MAIN),
                                     m_displaySize(50, 50),
                                     m_baseAlignment(ALIGN_TOP_LEFT)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ImagedAnimation::ImagedAnimation(Application* app, const String& name) : Object(app), 
                                                                         m_state(STATE_STOPPED), 
                                                                         m_name(name),
                                                                         m_baseRenderPriority(EGEGraphics::RP_MAIN),
                                                                         m_displaySize(50, 50),
                                                                         m_baseAlignment(ALIGN_TOP_LEFT)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ImagedAnimation::~ImagedAnimation()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ImagedAnimation::play(const String& sequencerName)
{
  // check if resumed
  if (isPaused())
  {
    // check if resuming the same sequencer
    if ((NULL != m_currentSequencer) && (sequencerName == m_currentSequencer->name()))
    {
      // change state
      m_state = STATE_PLAYING;
    }
    else
    {
      // stop it so it can be restarted from the begining
      stop();
    }
  }
  
  // get sequencer
  m_currentSequencer = this->sequencer(sequencerName);
  if (NULL == m_currentSequencer)
  {
    // error!
    return EGE_ERROR_NOT_FOUND;
  }

  if (!isPlaying() && !m_objects.empty() && !m_frames.empty() && (NULL != m_currentSequencer))
  {
    // setup sequencer
    m_currentSequencer->setFrameDuration(m_frameDuration);

    // connect to sequencer
    ege_connect(m_currentSequencer, frameChanged, this, ImagedAnimation::onSequencerFrameChanged);
    ege_connect(m_currentSequencer, finished, this, ImagedAnimation::onSequencerFinished);

    // change state
    m_state = STATE_PLAYING;

    // start sequencer
    m_currentSequencer->start();
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ImagedAnimation::play(s32 sequencerIndex)
{
  // check if replay requested
  if (0 > sequencerIndex)
  {
    // check if replay possible
    if (NULL != m_currentSequencer)
    {
      // replay
      return play(m_currentSequencer->name());
    }

    return EGE_ERROR_NOT_FOUND;
  }

  PSequencer seq = m_sequencers.at(sequencerIndex, NULL);
  if (NULL == seq)
  {
    // error!
    return EGE_ERROR_NOT_FOUND;
  }

  return play(seq->name());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ImagedAnimation::stop()
{
  if (isPlaying())
  {
    // disconnect from sequencer
    ege_disconnect(m_currentSequencer, frameChanged, this, ImagedAnimation::onSequencerFrameChanged);
    ege_disconnect(m_currentSequencer, finished, this, ImagedAnimation::onSequencerFinished);

    // change state
    m_state = STATE_STOPPED;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ImagedAnimation::pause()
{
  // change state
  m_state = STATE_PAUSED;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ImagedAnimation::isPlaying() const
{
  return STATE_PLAYING == m_state;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ImagedAnimation::isPaused() const
{
  return STATE_PAUSED == m_state;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ImagedAnimation::isStopped() const
{
  return STATE_STOPPED == m_state;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ImagedAnimation::update(const Time& time)
{
  if (isPlaying())
  {
    // update sequencer
    m_currentSequencer->update(time);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ImagedAnimation::setFPS(float32 fps)
{
  m_frameDuration = (0.0f < fps) ? (1.0f / fps) : 0.0f;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ImagedAnimation::setName(const String& name)
{
  m_name = name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ImagedAnimation::addObject(const EGEImagedAnimation::Object& object)
{
  ObjectData data;

  // check if object already in
  if (m_objects.contains(object.id))
  {
    // error!
    return EGE_ERROR_ALREADY_EXISTS;
  }

  for (EGEImagedAnimation::ChildObjectList::const_iterator itChild = object.children.begin(); itChild != object.children.end(); ++itChild)
  {
    const EGEImagedAnimation::ChildObject& childData = *itChild;

    ChildObjectData child;

    // create object
    child.baseFrameMatrix  = Matrix4f::IDENTITY;
    child.baseMatrix       = childData.matrix;
    child.renderData       = RenderObjectFactory::CreateQuadXY(app(), String::Format("image-animation-object-%d", m_objects.size()), Vector4f::ZERO, 
                                                               childData.size, ALIGN_TOP_LEFT, EGEVertexBuffer::ST_V2_T2, 0, EGEGraphics::RPT_TRIANGLE_STRIPS, 
                                                               EGEVertexBuffer::UT_DYNAMIC_WRITE_DONT_CARE);

    if (NULL == child.renderData)
    {
      // error!
      return EGE_ERROR_NO_MEMORY;
    }

    // set material
    child.renderData->setMaterial(childData.material);

    // add to pool
    data.children << child;
  }

  // add into pool
  m_objects.insert(object.id, data);
  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ImagedAnimation::addFrameData(const List<EGEImagedAnimation:: ActionData>& actions)
{
  FrameData frameData;
  frameData.actions = actions;

  m_frames.push_back(frameData);

  // if first frame added, update object
  if (1 == m_frames.size())
  {
   // update(0);
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ImagedAnimation::addForRendering(Renderer* renderer, const Matrix4f& transform)
{
  // check if no sequencer
  if (NULL == m_currentSequencer)
  {
    // do nothing
    return;
  }

  EGE_ASSERT(m_currentSequencer->frameId(m_currentSequencer->currentFrameIndex()) < static_cast<s32>(m_frames.size()));

  u32 count = 0;

  const FrameData& frameData = m_frames[m_currentSequencer->frameId(m_currentSequencer->currentFrameIndex())];
  for (List<EGEImagedAnimation::ActionData>::const_iterator it = frameData.actions.begin(); it != frameData.actions.end(); ++it)
  {
    const EGEImagedAnimation::ActionData& action = *it;

    const ObjectData& objectData = m_objects.at(action.objectId);

    // go thru all object children
    for (List<ChildObjectData>::const_iterator itChild = objectData.children.begin(); itChild != objectData.children.end(); ++itChild)
    {
      const ChildObjectData& childData = *itChild;

      // update priority
      childData.renderData->setPriority(m_baseRenderPriority + count);

      renderer->addForRendering(childData.renderData, transform * childData.baseFrameMatrix);

      ++count;
      //  EGE_PRINT("%f %f", objectData.baseFrameMatrix.translation().x, objectData.baseFrameMatrix.translation().y);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ImagedAnimation::setBaseRenderPriority(s32 priority)
{
  m_baseRenderPriority = priority;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ImagedAnimation::setDisplaySize(const Vector2f& size)
{
  m_displaySize = size;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const Vector2f& ImagedAnimation::displaySize() const
{
  return m_displaySize;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ImagedAnimation::setBaseAlignment(Alignment alignment)
{
  m_baseAlignment = alignment;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ImagedAnimation::clear()
{
  m_objects.clear();
  m_frames.clear();
  m_sequencers.clear();

  // disconnect from sequencer
  if (NULL != m_currentSequencer)
  {
    ege_disconnect(m_currentSequencer, frameChanged, this, ImagedAnimation::onSequencerFrameChanged);
    ege_disconnect(m_currentSequencer, finished, this, ImagedAnimation::onSequencerFinished);
  }

  m_currentSequencer = NULL;
  m_state            = STATE_STOPPED;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ImagedAnimation::addSequencer(const PSequencer& sequencer)
{
  // check if sequencer with such name exists
  if (NULL != this->sequencer(sequencer->name()))
  {
    // cannot add
    egeWarning() << "Sequencer with name:" << sequencer->name() << "already exists";
    return;
  }

  // add to pool
  m_sequencers.push_back(sequencer);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PSequencer ImagedAnimation::sequencer(const String& name) const
{
  // go thru all sequencers
  for (SequencerArray::const_iterator it = m_sequencers.begin(); it != m_sequencers.end(); ++it)
  {
    const PSequencer& current = *it;
    if (current->name() == name)
    {
      // found
      return current;
    }
  }

  // not found
  return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ImagedAnimation::onSequencerFrameChanged(PSequencer sequencer, s32 frameId)
{
  s32 nextFrameId = sequencer->frameId(Math::Min(sequencer->currentFrameIndex() + 1, sequencer->frameCount() - 1));

  // update object data
  FrameData& frameData            = m_frames[frameId];
  const FrameData& nextFrameData  = m_frames[nextFrameId];
  for (List<EGEImagedAnimation::ActionData>::iterator it = frameData.actions.begin(); it != frameData.actions.end(); ++it)
  {
    EGEImagedAnimation::ActionData& action = *it;

    ObjectData& objectData = m_objects.at(action.objectId);

    for (List<ChildObjectData>::iterator itChild = objectData.children.begin(); itChild != objectData.children.end(); ++itChild)
    {
      ChildObjectData& childData = *itChild;

      // update matrix
      childData.baseFrameMatrix = action.matrix * childData.baseMatrix;

      // update color
      childData.renderData->material()->setDiffuseColor(action.color);

      // apply alignment
      Vector4f translation = childData.baseFrameMatrix.translation();
      Math::Align(&translation, &m_displaySize, ALIGN_TOP_LEFT, m_baseAlignment);
      childData.baseFrameMatrix.setTranslation(translation.x, translation.y, translation.z);
  
      // set clipping rect
     // objectData.renderData->setClipRect(Rectf(translation.x, translation.y, m_displaySize.x, m_displaySize.y));
    }
  }

  // emit
  emit frameChanged(this, frameId);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ImagedAnimation::onSequencerFinished(PSequencer sequencer)
{
  // stop
  m_state = STATE_STOPPED;

  // emit
  emit finished(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PSequencer ImagedAnimation::currentSequencer() const 
{ 
  return m_currentSequencer; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END