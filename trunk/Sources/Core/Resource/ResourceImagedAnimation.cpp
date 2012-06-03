#include "Core/Resource/ResourceImagedAnimation.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Resource/ResourceSequencer.h"
#include "Core/Graphics/ImagedAnimation/ImagedAnimation.h"
#include <EGEXml.h>
#include <EGEDebug.h>
#include <EGEResources.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define NODE_OBJECT   "object"
#define NODE_FRAME    "frame"
#define NODE_ACTION   "action"
#define NODE_SEQUENCE "sequence"
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
  m_name        = tag->attribute("name");
  m_fps         = tag->attribute("fps").toFloat(&error);
  m_displaySize = tag->attribute("size").toVector2f(&error);

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
    else if (NODE_SEQUENCE == child->name())
    {
      // add sequence
      result = addSequence(child);
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

  // check if all sequencers are loaded
  for (SequenceResourceList::const_iterator it = m_sequenceResources.begin(); it != m_sequenceResources.end(); ++it)
  {
    PResourceSequencer seqResource = *it;
    if (!seqResource->isLoaded())
    {
      // error!
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

    // load all sequencers
    for (SequenceResourceList::iterator it = m_sequenceResources.begin(); it != m_sequenceResources.end(); ++it)
    {
      PResourceSequencer seqResource = *it;
      if (EGE_SUCCESS != (result = seqResource->load()))
      {
        // error!
        return result;
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

  //m_sequenceResources.clear();
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
  if ((NULL == instance) || !isLoaded())
  {
    return EGE_ERROR;
  }

  // clear instance
  instance->clear();

  // add objects
  for (ObjectDataArray::const_iterator it = m_objects.begin(); it != m_objects.end(); ++it)
  {
    const ObjectData& data = *it;

    Matrix4f matrix = Matrix4f::IDENTITY;
    matrix.setTranslation(data.translate.x, data.translate.y, 0);
    matrix.setScale(data.scale.x, data.scale.y, 1);
    matrix[0][1] = data.skew.x;
    matrix[1][0] = data.skew.y;
    
    if (EGE_SUCCESS != instance->addObject(data.id, data.size, data.materialResource->createInstance(), matrix))
    {
      // error!
      return EGE_ERROR;
    }
  }

  // add frames
  List<EGEImagedAnimation::ActionData> actions;
  for (FrameDataList::const_iterator itFrame = m_frames.begin(); itFrame != m_frames.end(); ++itFrame)
  {
    const FrameData& frameData = *itFrame;
    for (FrameActionDataList::const_iterator itAction = frameData.actions.begin(); itAction != frameData.actions.end(); ++itAction)
    {
      const FrameActionData& action = *itAction;

      EGEImagedAnimation::ActionData actionData;
          
      actionData.queue    = action.queue;
      actionData.objectId = action.objectId;
      actionData.matrix   = Matrix4f::IDENTITY;
      actionData.matrix.setTranslation(action.translate.x, action.translate.y, 0);
      actionData.matrix.setScale(action.scale.x, action.scale.y, 1);
      actionData.matrix[0][1] = action.skew.x;
      actionData.matrix[1][0] = action.skew.y;

      // add to pool
      actions.push_back(actionData);
    }

    if (EGE_SUCCESS != instance->addFrameData(actions))
    {
      // error!
      return EGE_ERROR;
    }

    // clean up
    actions.clear();
  }
  
  // setup data
  instance->setFPS(m_fps);
  instance->setName(name());
  instance->setDisplaySize(m_displaySize);

  // add sequencers
  for (SequenceResourceList::iterator it = m_sequenceResources.begin(); it != m_sequenceResources.end(); ++it)
  {
    PResourceSequencer seqRes = *it;

    // create instance of sequencer
    PSequencer seq = seqRes->createInstance();
    if (NULL == seq)
    {
      // error!
      return EGE_ERROR_NO_MEMORY;
    }

    instance->addSequencer(seq);
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Adds object. */
EGEResult ResourceImagedAnimation::addObject(const PXmlElement& tag)
{
  ObjectData data;

  bool error = false;

  // get data
  data.id           = tag->attribute("id").toInt(&error);
  data.materialName = tag->attribute("material");
  data.translate    = tag->attribute("translate", "0 0").toVector2f(&error);
  data.scale        = tag->attribute("scale", "1 1").toVector2f(&error);
  data.skew         = tag->attribute("skew", "0 0").toVector2f(&error);
  data.rect         = tag->attribute("rect", "0 0 0 0").toRectf(&error);
  data.size         = tag->attribute("size", "0 0").toVector2f(&error);

  if (error)
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
  EGEResult result = EGE_SUCCESS;

  FrameData data;

  bool error = false;

  // go thru all sub nodes
  PXmlElement child = tag->firstChild();
  while (child->isValid())
  {
    // check child
    if (NODE_ACTION == child->name())
    {
      // add action
      result = addAction(child, &data);
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

  m_frames.push_back(data);
  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Adds action to the given frame. */
EGEResult ResourceImagedAnimation::addAction(const PXmlElement& tag, FrameData* frameData) const
{
  bool error = false;

  FrameActionData action;

  // get data
  action.objectId   = tag->attribute("object-id").toInt(&error);
  action.queue      = tag->attribute("queue").toInt(&error);
  action.translate  = tag->attribute("translate", "0 0").toVector2f(&error);
  action.scale      = tag->attribute("scale", "1 1").toVector2f(&error);
  action.skew       = tag->attribute("skew", "0 0").toVector2f(&error);

  if (error)
  {
    // error!
    return EGE_ERROR;
  }

  // add to frame
  frameData->actions.push_back(action);

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Adds sequence. */
EGEResult ResourceImagedAnimation::addSequence(const PXmlElement& tag)
{
  EGEResult result = EGE_SUCCESS;

  // create sequence resource manually
  PResourceSequencer seqRes = ResourceSequencer::Create(app(), manager());
  if (NULL == seqRes)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  // deserialize resource
  if (EGE_SUCCESS != (result = seqRes->create("", tag)))
  {
    // error!
    return result;
  }

  // check if such sequence already exists
  for (SequenceResourceList::const_iterator it = m_sequenceResources.begin(); it != m_sequenceResources.end(); ++it)
  {
    const PResourceSequencer& resource = *it;
    if (resource->name() == seqRes->name())
    {
      // error!
      return EGE_ERROR_ALREADY_EXISTS;
    }
  }

  // add to pool
  m_sequenceResources.push_back(seqRes);

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END