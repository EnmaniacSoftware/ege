#include "Core/Resource/Interface/Loader/ResourceLoaderXML.h"
#include "Core/Resource/Interface/ResourceGroup.h"
#include "EGEDebug.h"
#include "EGEDirectory.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const char* KResourceLoaderXMLName = "xml";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const char* KResourceLoaderXMLDebugName = "EGEResourceLoaderXML";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const char* KNodeResources = "resources";
static const char* KNodeGroup     = "group";
static const char* KNodeInclude   = "include";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ResourceLoaderXML)
EGE_DEFINE_DELETE_OPERATORS(ResourceLoaderXML)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceLoaderXML::ResourceLoaderXML(Engine& engine) 
: m_engine(engine)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceLoaderXML::~ResourceLoaderXML()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceLoaderXML::addResources(const String& filePath)
{
  EGEResult result = EGE_SUCCESS;

  XmlDocument xml;
  if (EGE_SUCCESS != (result = xml.load(filePath)))
  {
    // not found
    result = EGE_ERROR_NOT_FOUND;
  }
  else
  {
    // get main node
    PXmlElement resourcesNode = xml.firstChild(KNodeResources);
    if ((NULL == resourcesNode) || ! resourcesNode->isValid())
    {
      // error!
      egeWarning(KResourceLoaderXMLDebugName) << "Resource file" << filePath << "has no" << KNodeResources << "tag";
      result = EGE_ERROR;
    }
    else
    {
      // process RESOURCES tag
      String path;
      String file;
      Directory::DecomposePath(filePath, path, file);
      result = processResourcesTag(path, resourcesNode);
    }
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceLoaderXML::processResourcesTag(const String& filePath, const PXmlElement& tag)
{
  EGEResult result = EGE_SUCCESS;

  // go thru all children
  PXmlElement child = tag->firstChild();
  while (child->isValid())
  {
    // get child name
    const String childName = child->name();

    // process GROUP tag
    if (KNodeGroup == childName)
    {
      result = processGroup(filePath, child);
    }
    // process INCLUDE tag
    else if (KNodeInclude == childName)
    {
      result = processInclude(filePath, child);
    }

    // check if error occured
    if (EGE_SUCCESS != result)
    {
      // we are done
      break;
    }

    // go to next child
    child = child->nextChild();
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceLoaderXML::processGroup(const String& filePath, const PXmlElement& tag)
{
  EGEResult result = EGE_SUCCESS;

  PResourceGroup newGroup = ege_new ResourceGroup(m_engine);
  if (NULL == newGroup)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }
  
  // create from XML
  result = newGroup->create(filePath, tag);
  if (EGE_SUCCESS == result)
  {
    egeDebug(KResourceLoaderXMLDebugName) << newGroup->name();

    // check if such group DOES NOT exists
    PResourceGroup existingGroup = m_engine.resourceManager()->group(newGroup->name());
    if (NULL == existingGroup)
    {
      // notify 
      emit signalGroupCreated(newGroup);
    }
    else
    {
      // try to override
      result = existingGroup->overrideBy(newGroup);
      if (EGE_ERROR_NOT_SUPPORTED == result)
      {
        // error!
        egeWarning(KResourceLoaderXMLDebugName) << "Attempt to override non-overridable group" << existingGroup->name();
      }
      else if (EGE_ERROR_ALREADY_EXISTS == result)
      {
        // NOTE: we quitely omit group duplicates so it is valid to ie. INCLUDE the same group multiple times
        egeWarning(KResourceLoaderXMLDebugName) << "Group" << newGroup->name() << "already exists. Skipping.";
        result = EGE_SUCCESS;
      }
    }
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceLoaderXML::processInclude(const String& filePath, const PXmlElement& tag)
{
  bool error = false;

  // get data
  String path     = tag->attribute("path");
  bool autoDetect = tag->attribute("auto-detect", "false").toBool(&error);

  // check if obligatory data is wrong
  if (path.isEmpty() || error)
  {
    // error!
    return EGE_ERROR;
  }

  egeDebug(KResourceLoaderXMLDebugName) << "Including" << path;

  // check if not autodetecting
  // NOTE: in this case we assume path is with respect to current directory
  if ( ! autoDetect)
  {
    // compose absolute path
    path = Directory::Join(filePath, path);
  }

  // add new resource
  return m_engine.resourceManager()->addResources(path, autoDetect);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END