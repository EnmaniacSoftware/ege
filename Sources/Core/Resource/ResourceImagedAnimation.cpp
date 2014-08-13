#include "Core/Resource/ResourceImagedAnimation.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Resource/ResourceSequencer.h"
#include "EGEImagedAnimation.h"
#include "EGEXml.h"
#include "EGEResources.h"
#include "EGEStringUtils.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const char* KResourceImagedAnimationDebugName = "EGEResourceImagedAnimation";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define NODE_OBJECT   "object"
#define NODE_FRAME    "frame"
#define NODE_ACTION   "action"
#define NODE_SEQUENCE "sequence"
#define NODE_CHILD    "child"
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ResourceImagedAnimation)
EGE_DEFINE_DELETE_OPERATORS(ResourceImagedAnimation)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceImagedAnimation::ResourceImagedAnimation(Engine& engine, ResourceGroup* group) : IResource(engine, group, RESOURCE_NAME_IMAGED_ANIMATION)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceImagedAnimation::~ResourceImagedAnimation()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PResource ResourceImagedAnimation::Create(Engine& engine, ResourceGroup* group)
{
  return ege_new ResourceImagedAnimation(engine, group);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const String& ResourceImagedAnimation::name() const
{
  return m_name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceImagedAnimation::create(const String& path, const PXmlElement& tag)
{
  EGE_UNUSED(path);

  EGEResult result = EGE_SUCCESS;

  bool error = false;

  // get data
  m_name        = tag->attribute("name");
  m_fps         = tag->attribute("fps").toFloat(&error);
  m_displaySize = StringUtils::ToVector2f(tag->attribute("size"), &error);

  // check if obligatory data is wrong
  if (error || m_name.empty())
  {
    // error!
    egeWarning(KResourceImagedAnimationDebugName) << "Failed for name:" << m_name;
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

  // check if success
  if (EGE_SUCCESS == result)
  {
    // set state
    m_state = STATE_UNLOADED;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceImagedAnimation::load()
{
  EGEResult result = EGE_SUCCESS;

  if (STATE_LOADED != m_state)
  {
    // load all objects materials
    for (ObjectDataArray::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
    {
      ObjectData& dataObject = *it;

      for (ObjectChildDataList::iterator itChild = dataObject.children.begin(); itChild != dataObject.children.end(); ++itChild)
      {
        ObjectChildData& childData = *itChild;

        childData.materialResource = group()->manager()->resource(RESOURCE_NAME_MATERIAL, childData.materialName);
        if (childData.materialResource)
        {
          // load material
          if (EGE_SUCCESS != (result = childData.materialResource->load()))
          {
            // error!
            childData.materialResource = NULL;
            return result;
          }
        }
        else
        {
          // material not found
          return EGE_ERROR_NOT_FOUND;
        }
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

    // set flag
    m_state = STATE_LOADED;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceImagedAnimation::unload() 
{ 
  // unload all objects materials
  for (ObjectDataArray::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
  {
    ObjectData& dataObject = *it;

    for (ObjectChildDataList::iterator itChild = dataObject.children.begin(); itChild != dataObject.children.end(); ++itChild)
    {
      ObjectChildData& childData = *itChild;

      childData.materialResource = NULL;
    }
  }

  //m_sequenceResources.clear();

  // reset flag
  m_state = STATE_UNLOADED;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PImagedAnimation ResourceImagedAnimation::createInstance()
{
	PImagedAnimation object = ege_new ImagedAnimation(engine(), name());
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
EGEResult ResourceImagedAnimation::setInstance(const PImagedAnimation& instance)
{
  // sanity check
  if ((NULL == instance) || (STATE_LOADED != m_state))
  {
    // error!
    return EGE_ERROR;
  }

  // clear instance
  instance->clear();

  // add objects
  Map<s32, EGEImagedAnimation::Object> objects;
  for (ObjectDataArray::const_iterator it = m_objects.begin(); it != m_objects.end(); ++it)
  {
    const ObjectData& data = *it;

    EGEImagedAnimation::Object object;
    object.id = data.id;

    // go thru all children
    for (ObjectChildDataList::const_iterator itChild = data.children.begin(); itChild != data.children.end(); ++itChild)
    {
      const ObjectChildData& childData = *itChild;

      EGEImagedAnimation::ChildObject child;

      child.matrix = Matrix4f::IDENTITY;
      child.matrix.setTranslation(childData.translate.x, childData.translate.y, 0);
      child.matrix.setScale(childData.scale.x, childData.scale.y, 1);
      child.matrix[0][1] = childData.skew.x;
      child.matrix[1][0] = childData.skew.y;
      child.size = childData.size;
      child.material = childData.materialResource->createInstance();
      child.rect = child.material->pass(0)->texture(0)->rect();
    
      object.children << child;
    }

    objects.insert(object.id, object);
  }

  // add frames
  List<List<EGEImagedAnimation::ActionData> > allActions;
  for (FrameDataList::const_iterator itFrame = m_frames.begin(); itFrame != m_frames.end(); ++itFrame)
  {
    const FrameData& frameData = *itFrame;

    List<EGEImagedAnimation::ActionData> actions;
    for (FrameActionDataList::const_iterator itAction = frameData.actions.begin(); itAction != frameData.actions.end(); ++itAction)
    {
      const FrameActionData& action = *itAction;

      EGEImagedAnimation::ActionData actionData;
          
      actionData.objectId = action.objectId;
      actionData.matrix   = Matrix4f::IDENTITY;
      actionData.matrix.setTranslation(action.translate.x, action.translate.y, 0);
      actionData.matrix.setScale(action.scale.x, action.scale.y, 1);
      actionData.matrix[0][1] = action.skew.x;
      actionData.matrix[1][0] = action.skew.y;
      actionData.color    = action.color;

      // add to pool
      actions.push_back(actionData);
    }

    allActions << actions;

    // clean up
    actions.clear();
  }
  
  instance->setKeysPerSecond(m_fps);
  instance->setName(name());
  instance->setDisplaySize(m_displaySize);
  instance->addData(objects, allActions);

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
EGEResult ResourceImagedAnimation::addObject(const PXmlElement& tag)
{
  ObjectData data;

  bool error = false;

  // get data
  data.id = tag->attribute("id").toInt(&error);

  if (error)
  {
    // error!
    return EGE_ERROR;
  }

  // go thru all sub nodes
  PXmlElement child = tag->firstChild();
  while (child->isValid())
  {
    error = false;

    // check child
    if (NODE_CHILD == child->name())
    {
      ObjectChildData childData;

      childData.materialName = child->attribute("material");
      childData.translate    = StringUtils::ToVector2f(child->attribute("translate", "0 0"), &error);
      childData.scale        = StringUtils::ToVector2f(child->attribute("scale", "1 1"), &error);
      childData.skew         = StringUtils::ToVector2f(child->attribute("skew", "0 0"), &error);
      childData.rect         = StringUtils::ToRectf(child->attribute("rect", "0 0 0 0"), &error);
      childData.size         = StringUtils::ToVector2f(child->attribute("size", "0 0"), &error);

      if (error)
      {
        // error!
        return EGE_ERROR;
      }

      // add to pool
      data.children << childData;
    }

    // go to next child
    child = child->nextChild();
  }

  m_objects.push_back(data);
  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceImagedAnimation::addFrame(const PXmlElement& tag)
{
  EGEResult result = EGE_SUCCESS;

  FrameData data;

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
EGEResult ResourceImagedAnimation::addAction(const PXmlElement& tag, FrameData* frameData) const
{
  bool error = false;

  FrameActionData action;

  // get data
  action.objectId   = tag->attribute("object-id").toInt(&error);
 // action.queue      = tag->attribute("queue").toInt(&error);
  action.translate  = StringUtils::ToVector2f(tag->attribute("translate", "0 0"), &error);
  action.scale      = StringUtils::ToVector2f(tag->attribute("scale", "1 1"), &error);
  action.skew       = StringUtils::ToVector2f(tag->attribute("skew", "0 0"), &error);
  action.color      = StringUtils::ToColor(tag->attribute("color", "1 1 1 1"), &error);

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
EGEResult ResourceImagedAnimation::addSequence(const PXmlElement& tag)
{
  EGEResult result = EGE_SUCCESS;

  // create sequence resource manually
  PResourceSequencer seqRes = ResourceSequencer::Create(engine(), group());
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