#include "Core/Resource/ResourceSound.h"
#include "Core/Resource/ResourceManager.h"
#include <EGEApplication.h>
#include <EGEGraphics.h>
#include <EGEXml.h>
#include <EGEDebug.h>
#include <EGEResources.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(ResourceSound)
EGE_DEFINE_DELETE_OPERATORS(ResourceSound)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceSound::ResourceSound(Application* app, ResourceManager* manager) : IResource(app, manager, RESOURCE_NAME_SOUND)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceSound::~ResourceSound()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance of resource. This method is a registration method for manager. */
PResource ResourceSound::Create(Application* app, ResourceManager* manager)
{
  return ege_new ResourceSound(app, manager);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Returns name of resource. */
const String& ResourceSound::name() const
{
  return m_name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Initializes resource from XML. 
* 
*  \param  path  full path to resource definition file.
*  \param  tag   xml element with resource definition. 
*/
EGEResult ResourceSound::create(const String& path, const PXmlElement& tag)
{
  EGEResult result = EGE_SUCCESS;

  bool error = false;

  // get data
  m_name    = tag->attribute("name");
  m_path    = tag->attribute("path");
  m_pitch   = tag->attribute("pitch", "1.0").toFloat(&error);
  m_gain    = tag->attribute("gain", "1.0").toFloat(&error);
  m_looping = tag->attribute("looping", "false").toBool(&error);

  // check if obligatory data is wrong
  if (m_name.empty() || m_path.empty() || error)
  {
    // error!
    EGE_PRINT("ResourceSound::create - failed for name: %s", m_name.toAscii());
    return EGE_ERROR_BAD_PARAM;
  }

  // compose full path
  m_path = path + "/" + m_path;

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Loads resource. */
EGEResult ResourceSound::load()
{
  // nothing to do
  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Unloads resource. */
void ResourceSound::unload() 
{ 
  EGE_PRINT("ResourceSound::unload - %s", name().toAscii());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance of sound object defined by resource. */
PSound ResourceSound::createInstance()
{
  // open sound file for reading
  File file(m_path);
  if (EGE_SUCCESS != file.open(EGEFile::MODE_READ_ONLY))
  {
    // error!
    return NULL;
  }

  // find whats file size
  s64 fileSize = file.size();

  // read file data
  DataBuffer data;
  if (file.read(data, fileSize) != fileSize)
  {
    // error!
    return NULL;
  }

  // create sound object from data
  PSound object = ege_new Sound(data);

  // set data
  object->setPitch(m_pitch);
  object->setGain(m_gain);
  object->setLooping(m_looping);

  return object;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
