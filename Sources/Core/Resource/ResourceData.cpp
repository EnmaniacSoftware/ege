#include "Core/Resource/ResourceData.h"
#include "Core/Resource/ResourceManager.h"
#include "EGEFile.h"
#include "EGEFileUtils.h"
#include "EGEResources.h"
#include "EGEDirectory.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const char* KResourceDataDebugName = "ResourceData";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ResourceData)
EGE_DEFINE_DELETE_OPERATORS(ResourceData)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceData::ResourceData(Engine& engine, ResourceGroup* group) 
: IResource(engine, group, RESOURCE_NAME_DATA)
, m_nulled(false)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceData::~ResourceData()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PResource ResourceData::Create(Engine& engine, ResourceGroup* group)
{
  return ege_new ResourceData(engine, group);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const String& ResourceData::name() const
{
  return m_name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceData::create(const String& path, const PXmlElement& tag)
{
  EGEResult result = EGE_SUCCESS;

  // get data
  bool error = false;
  m_name   = tag->attribute("name");
  m_path   = tag->attribute("path");
  m_nulled = tag->attribute("nulled").toBool(&error);

  // check if obligatory data is wrong
  if (m_name.empty() || m_path.empty() || error)
  {
    // error!
    egeWarning(KResourceDataDebugName) << "Failed for name:" << m_name;
    return EGE_ERROR_BAD_PARAM;
  }

  // compose full path
  m_path = Directory::Join(path, m_path);

  // set state
  m_state = STATE_UNLOADED;

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceData::load()
{
  EGEResult result = EGE_SUCCESS;

  if (STATE_LOADED != m_state)
  {
    PDataBuffer buffer = ege_new DataBuffer();
    if (NULL == buffer)
    {
      // error!
      return EGE_ERROR_NO_MEMORY;
    }

    // read file data into buffer
    File file(path());
    if (EGE_SUCCESS != (result = file.open(EFileModeReadOnly)))
    {
      // error!
      return result;
    }

    // get file size
    s64 size = FileUtils::Size(file.filePath());

    // read entire file
    if ((-1 == size) || (size != file.read(buffer, size)))
    {
      // error!
      return EGE_ERROR_IO;
    }

    file.close();

    // check if data should be null terminated
    if (isNulled())
    {
      s8 null = 0;
      if (1 != buffer->write(&null, 1))
      {
        // error!
        return EGE_ERROR;
      }
    }

    // success
    m_data = buffer;

    // set flag
    m_state = STATE_LOADED;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceData::unload()
{
  // clean up
  m_data = NULL;

  // reset flag
  m_state = STATE_UNLOADED;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

 EGE_NAMESPACE_END