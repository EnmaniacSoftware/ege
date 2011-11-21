#include "Core/Resource/ResourceText.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Application/Application.h"
#include <EGEFile.h>
#include <EGEResources.h>
#include <EGEMath.h>

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
/*! Returns text translation. */
Text ResourceText::text(s32 numerous) const
{
  Text outText;

  // detrmine translation index based on given numerous
  s32 index = (-1 != numerous) ? translationIndex(numerous) : 0;

  // check if translations for current language exists
  if (m_translations.contains(app()->language()))
  {
    const TextArray& list = m_translations.at(app()->language());

    index = Math::Bound(index, static_cast<s32>(0), static_cast<s32>(list.size()) - 1);
    outText = list[index];
  }
  else
  {
    // try to fallback to english
    if (m_translations.contains("en"))
    {
      // get singular form just for indication that we could not find anything
      outText = m_translations.at("en")[0];
    }
  }

  // replace %n
  if (-1 != numerous)
  {
    Text number   = Text::Format("%d", numerous);
    Text percentN = EGE_TEXT("%n");

    size_t pos = outText.find(percentN);
    while (Text::npos != pos)
    {
      // replace
      outText.replace(pos, 2, number);

      // find next
      pos = outText.find(percentN, pos + 2);
    }
  }

  return outText;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns index of translation for given numerous. */
s32 ResourceText::translationIndex(s32 numerous) const
{
  s32 index = 0;

  // TAGE - this probably should end up in some Language class
  if ("en" == app()->language())
  {
    switch (numerous)
    {
      case 1: 
        
        // singular
        index = 0; 
        break;
      
      default: 

        // plural and zero
        index = 1;
        break;
    }
  }
  else if ("pl" == app()->language())
  {
    switch (numerous)
    {
      case 1: 
        
        // singular
        index = 0; 
        break;
      
      case 2:
      case 3:
      case 4:

        // plural, case 1
        index = 1;
        break;

      default:

        // zero, plural, case 2
        index = 2;
        break;
    }
  }

  return index;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
