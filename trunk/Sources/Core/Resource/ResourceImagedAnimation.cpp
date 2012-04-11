#include "Core/Resource/ResourceImagedAnimation.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Animation/ImagedAnimation.h"
#include <EGEXml.h>
#include <EGEDebug.h>
#include <EGEResources.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define NODE_OBJECT "object"
#define NODE_FRAME  "frame"
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ResourceImagedAnimation)
EGE_DEFINE_DELETE_OPERATORS(ResourceImagedAnimation)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceImagedAnimation::ResourceImagedAnimation(Application* app, ResourceManager* manager) : IResource(app, manager, RESOURCE_NAME_IMAGED_ANIMATION)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceImagedAnimation::~ResourceImagedAnimation()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance of resource. This method is a registration method for manager. */
PResource ResourceImagedAnimation::Create(Application* app, ResourceManager* manager)
{
  return ege_new ResourceImagedAnimation(app, manager);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Returns name of resource. */
const String& ResourceImagedAnimation::name() const
{
  return m_name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Initializes resource from XML. 
* 
*  \param  path  full path to resource definition file.
*  \param  tag   xml element with resource definition. 
*/
EGEResult ResourceImagedAnimation::create(const String& path, const PXmlElement& tag)
{
  EGEResult result = EGE_SUCCESS;

  bool error = false;

  // get data
  m_name = tag->attribute("name");
  m_fps  = tag->attribute("fps").toInt(&error);

  // check if obligatory data is wrong
  if (error || m_name.empty())
  {
    // error!
    EGE_PRINT("ERROR: Failed for name: %s", m_name.toAscii());
    return EGE_ERROR_BAD_PARAM;
  }

  // go thru all sub nodes
  PXmlElement child = tag->firstChild();
  while (child->isValid())
  {
    // check child
    if (NODE_OBJECT == child->name())
    {
      // add object
      result = addObject(child);
    }
    else if (NODE_FRAME == child->name())
    {
      // add frame
      result = addFrame(child);
    }

    // check if failed
    if (EGE_SUCCESS != result)
    {
      // error, done!
      break;
    }

    // go to next child
    child = child->nextChild();
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is loaded. */
bool ResourceImagedAnimation::isLoaded() const
{
  // check if all materials are loaded
  for (s32 i = 0; i < static_cast<s32>(m_objects.size()); ++i)
  {
    if (NULL == m_objects[i].materialResource)
    {
      // not loaded
      return false;
    }
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Loads resource. */
EGEResult ResourceImagedAnimation::load()
{
  EGEResult result = EGE_SUCCESS;

  if (!isLoaded())
  {
    // load all objects materials
    for (s32 i = 0; i < static_cast<s32>(m_objects.size()); ++i)
    {
      ObjectData& objectData = m_objects[i];

      objectData.materialResource = manager()->resource(RESOURCE_NAME_MATERIAL, objectData.materialName);
      if (objectData.materialResource)
      {
        // load material
        if (EGE_SUCCESS != (result = objectData.materialResource->load()))
        {
          // error!
          objectData.materialResource = NULL;
          return result;
        }
      }
      else
      {
        // material not found
        result = EGE_ERROR_NOT_FOUND;
      }
    }
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Unloads resource. */
void ResourceImagedAnimation::unload() 
{ 
  EGE_PRINT("%s", name().toAscii());

  // unload all objects materials
  for (s32 i = 0; i < static_cast<s32>(m_objects.size()); ++i)
  {
    ObjectData& objectData = m_objects[i];

    objectData.materialResource = NULL;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance of imaged animation object defined by resource. */
PImagedAnimation ResourceImagedAnimation::createInstance()
{
	PImagedAnimation object = ege_new ImagedAnimation(app(), name());
  if (object)
  {
    // set new data
    if (EGE_SUCCESS != setInstance(object))
    {
      object = NULL;
    }
  }

  return object;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Set given instance of imaged animation object to what is defined by resource. */
EGEResult ResourceImagedAnimation::setInstance(const PImagedAnimation& instance)
{
  // sanity check
  if (NULL == instance || !isLoaded())
  {
    return EGE_ERROR;
  }

  // generate frame data
//  calculateFrameData();
  
  // setup data
  //instance->setDuration(m_duration);
  //instance->setFrameData(m_frameData);
  //instance->setTexture(m_sheet->textureImage()->createInstance());

  //Sprite::FinishPolicy finishPolicy = Sprite::FP_STOP;
  //if (m_repeat)
  //{
  //  finishPolicy = Sprite::FP_REPEAT;
  //}
  //else if (m_pingPong)
  //{
  //  finishPolicy = Sprite::FP_PING_PONG;
  //}
  //instance->setFinishPolicy(finishPolicy);

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
///*! Invalidates frame data. */
//void ResourceImagedAnimation::invalidateFrameData()
//{
//  m_frameDataInvalid = true;
//}
////--------------------------------------------------------------------------------------------------------------------------------------------------------------
///*! Calculates frame data. */
//void ResourceImagedAnimation::calculateFrameData()
//{
//  const float32 cellWidth  = 1.0f / sheet()->framesPerRow();
//  const float32 cellHeight = 1.0f / Math::Ceil(sheet()->frameCount() / static_cast<float32>(sheet()->framesPerRow()));
//
//  if (m_frameDataInvalid)
//  {
//    TextureImage textureImage(app());
//    m_sheet->textureImage()->setInstance(textureImage);
//
//    // create new data for each frame
//    FameDataArray allFramesData;
//    for (s32 i = 0; i < m_frameCount; ++i)
//    {
//      EGESprite::FrameData data;
//  
//      data.m_rect = textureImage.rect().combine(Rectf((m_beginFrame + i) % sheet()->framesPerRow() * cellWidth, 
//                                                      (m_beginFrame + i) / sheet()->framesPerRow() * cellHeight, cellWidth, cellHeight));
//
//      // add to pool
//      allFramesData.push_back(data);
//    }
//
//    // clear old data
//    m_frameData.clear();
//
//    // fill in frames for single cycle
//    if (m_repeat || (!m_repeat && !m_pingPong))
//    {
//      // for REPEAT and STOP modes, cycle consists of all frames from begin one to the end. After reaching end playback should repeat itself
//      for (s32 i = 0; i < m_frameCount; ++i)
//      {
//        m_frameData.push_back(allFramesData[i]);
//      }
//    }
//    else if (m_pingPong)
//    {
//      // for PING-PONG mode, cycle consists of all frames from begin one to the end and back. However, last frame should be counted only once and final frame
//      // should end before first frame so in next cycle first frame is only processed once (by new cycle only)
//      for (s32 i = 0; i < m_frameCount; ++i)
//      {
//        m_frameData.push_back(allFramesData[i]);
//      }
//
//      for (s32 i = m_frameCount - 1; i > 0; --i)
//      {
//        m_frameData.push_back(allFramesData[i]);
//      }
//    }
//    else
//    {
//      EGE_ASSERT(false && "Not implemented mode");
//    }
//
//    // validate
//    m_frameDataInvalid = false;
//  }
//}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Adds object. */
EGEResult ResourceImagedAnimation::addObject(const PXmlElement& tag)
{
  ObjectData data;

  bool error = false;

  // get data
  data.name       = tag->attribute("name");
  data.translate  = tag->attribute("translate", "0 0").toVector2f(&error);
  data.scale      = tag->attribute("scale", "1 1").toVector2f(&error);
  data.skew       = tag->attribute("skew", "0 0").toVector2f(&error);

  if (data.name.empty() || error)
  {
    // error!
    return EGE_ERROR;
  }

  m_objects.push_back(data);
  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Adds frame. */
EGEResult ResourceImagedAnimation::addFrame(const PXmlElement& tag)
{
  FrameData data;

  bool error = false;

  // get data
  data.objectName = tag->attribute("object");
  data.queue      = tag->attribute("queue").toInt(&error);
  data.translate  = tag->attribute("translate", "0 0").toVector2f(&error);
  data.scale      = tag->attribute("scale", "1 1").toVector2f(&error);
  data.skew       = tag->attribute("skew", "0 0").toVector2f(&error);

  if (data.objectName.empty() || error)
  {
    // error!
    return EGE_ERROR;
  }

  m_frames.push_back(data);
  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
