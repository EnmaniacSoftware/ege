#include "Core/Resource/ResourceSound.h"
#include "Core/Resource/ResourceManager.h"
#include "EGEApplication.h"
#include "EGEGraphics.h"
#include "EGEXml.h"
#include "EGEDebug.h"
#include "EGEResources.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ResourceSound)
EGE_DEFINE_DELETE_OPERATORS(ResourceSound)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceSound::ResourceSound(Application* app, ResourceGroup* group) : IResource(app, group, RESOURCE_NAME_SOUND)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceSound::~ResourceSound()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PResource ResourceSound::Create(Application* app, ResourceGroup* group)
{
  return ege_new ResourceSound(app, group);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const String& ResourceSound::name() const
{
  return m_name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
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

  // check if success
  if (EGE_SUCCESS == result)
  {
    // set state
    m_state = STATE_UNLOADED;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceSound::load()
{
  if ((STATE_LOADED != m_state))
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

    // set flag
    m_state = STATE_LOADED;
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceSound::unload() 
{ 
  // clean up
  m_data = NULL;

  // reset flag
  m_state = STATE_UNLOADED;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PSound ResourceSound::createInstance()
{
  if (STATE_LOADED != m_state)
  {
    // error!
    return NULL;
  }

  // create sound object from data
  PSound object = app()->audioManager()->createSound(name(), m_data);
  if ((NULL == object))
  {
    // error!
    return NULL;
  }

  // set data
  object->setPitch(m_pitch);

  return object;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END