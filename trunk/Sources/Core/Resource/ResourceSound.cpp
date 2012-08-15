#include "Core/Resource/ResourceSound.h"
#include "Core/Resource/ResourceManager.h"
#include <EGEApplication.h>
#include <EGEGraphics.h>
#include <EGEXml.h>
#include <EGEDebug.h>
#include <EGEResources.h>

EGE_NAMESPACE_BEGIN

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

  // check if obligatory data is wrong
  if (m_name.empty() || m_path.empty() || error)
  {
    // error!
    egeWarning() << "Failed for name:" << m_name;
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
  // allocate data buffer
  m_data = ege_new DataBuffer();
  if (NULL == m_data)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  // open sound file for reading
  File file(m_path);
  if (EGE_SUCCESS != file.open(EGEFile::MODE_READ_ONLY))
  {
    // error!
    return EGE_ERROR_IO;
  }

  // find whats file size
  s64 fileSize = file.size();

  // read data into buffer
  if (file.read(m_data, fileSize) != fileSize)
  {
    // error!
    return EGE_ERROR_IO;
  }

 // egeDebug() << "Loaded sound resource" << name();

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Unloads resource. */
void ResourceSound::unload() 
{ 
  //egeDebug() << "Unloading sound resource" << name();
  m_data = NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance of sound object defined by resource. */
PSound ResourceSound::createInstance()
{
  if (!isLoaded())
  {
    // error!
    return NULL;
  }

  // create sound object from data
  PSound object = ege_new Sound(name(), m_data);
  if ((NULL == object) || (EGE_SUCCESS != object->construct()))
  {
    // error!
    return NULL;
  }

  // set data
  object->setPitch(m_pitch);

  return object;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Returns TRUE if object is loaded. */
bool ResourceSound::isLoaded() const 
{ 
  return (NULL != m_data); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END