#include "Core/Resource/ResourceText.h"
#include "Core/Resource/ResourceManager.h"
#include <EGEFile.h>
#include <EGEResources.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define NODE_LANG "lang"
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(ResourceText)
EGE_DEFINE_DELETE_OPERATORS(ResourceText)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceText::ResourceText(Application* app, ResourceManager* manager) : IResource(app, manager, RESOURCE_NAME_TEXT)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceText::~ResourceText()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance of resource. This method is a registration method for manager. */
PResource ResourceText::Create(Application* app, ResourceManager* manager)
{
  return ege_new ResourceText(app, manager);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Returns name of resource. */
const String& ResourceText::name() const
{
  return m_name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Initializes resource from XML. 
* 
*  \param  path  full path to resource definition file.
*  \param  tag   xml element with resource definition. 
*/
EGEResult ResourceText::create(const String& path, const PXmlElement& tag)
{
  EGEResult result = EGE_SUCCESS;

  // get data
  bool error = false;
  m_name = tag->attribute("name");

  // check if obligatory data is wrong
  if (m_name.empty())
  {
    // error!
    EGE_PRINT("ResourceText::create - failed for name: %s", m_name.toAscii());
    return EGE_ERROR_BAD_PARAM;
  }

  // go thru all children
  PXmlElement child = tag->firstChild();
  while (child->isValid())
  {
    // check if language node
    if (NODE_LANG == child->name())
    {
      result = addLocalization(child);
    }

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
/*! IResource override. Loads resource. */
EGEResult ResourceText::load()
{
  // nothing to load
  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Unloads resource. */
void ResourceText::unload()
{
  // nothing to unload
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Adds text localization. */
EGEResult ResourceText::addLocalization(const PXmlElement& tag)
{
  // get data
  String name = tag->attribute("name");
  Text value  = tag->attribute("value");

  // check if data is present
  if (name.empty() || value.empty())
  {
    // error!
    return EGE_ERROR_BAD_PARAM;
  }

  // add to pool
  m_translations[name] << value;

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
