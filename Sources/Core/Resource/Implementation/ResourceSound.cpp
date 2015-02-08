#include "Core/Resource/Interface/ResourceSound.h"
#include "EGEEngine.h"
#include "EGEFileUtils.h"
#include "EGEGraphics.h"
#include "EGEXml.h"
#include "EGEDebug.h"
#include "EGEResources.h"
#include "EGEDirectory.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const char* KResourceSoundDebugName = "EGEResourceSound";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ResourceSound)
EGE_DEFINE_DELETE_OPERATORS(ResourceSound)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceSound::ResourceSound(Engine& engine, ResourceGroup* group) 
: IResource(engine, group, RESOURCE_NAME_SOUND)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceSound::~ResourceSound()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const String& ResourceSound::name() const
{
  return m_name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceSound::create(const String& path, const PObject& data)
{
  EGEResult result = EGE_SUCCESS;

  PXmlElement tag = ege_pcast<PXmlElement>(data);

  bool error = false;

  // get data
  m_name    = tag->attribute("name");
  m_path    = tag->attribute("path");
  m_pitch   = tag->attribute("pitch", "1.0").toFloat(&error);

  // check if obligatory data is wrong
  if (m_name.isEmpty() || m_path.isEmpty() || error)
  {
    // error!
    egeWarning(KResourceSoundDebugName) << "Failed for name:" << m_name;
    return EGE_ERROR_BAD_PARAM;
  }

  // compose full path
  m_path = Directory::Join(path, m_path);

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
    if (EGE_SUCCESS != file.open(EFileModeReadOnly))
    {
      // error!
      return EGE_ERROR_IO;
    }

    // find whats file size
    s64 fileSize = FileUtils::Size(file.filePath());

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
  PSound object = engine().audioManager()->createSound(name(), m_data);
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