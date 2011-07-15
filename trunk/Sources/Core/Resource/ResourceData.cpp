#include "Core/Resource/ResourceData.h"
#include "Core/Resource/ResourceManager.h"
#include <EGEFile.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(ResourceData)
EGE_DEFINE_DELETE_OPERATORS(ResourceData)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceData::ResourceData(Application* app, ResourceManager* manager) : IResource(app, manager, "data"), m_nulled(false)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceData::~ResourceData()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance of resource. This method is a registration method for manager. */
PResource ResourceData::Create(Application* app, ResourceManager* manager)
{
  return ege_new ResourceData(app, manager);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Returns name of resource. */
const String& ResourceData::name() const
{
  return m_name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Initializes resource from XML. 
* 
*  \param  path  full path to resource definition file.
*  \param  tag   xml element with resource definition. 
*/
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
    return EGE_ERROR_BAD_PARAM;
  }

  // compose full path
  m_path = path + "/" + m_path;

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Loads resource. */
EGEResult ResourceData::load()
{
  EGEResult result = EGE_SUCCESS;

  if (!isLoaded())
  {
    PDataBuffer buffer = ege_new DataBuffer();
    if (NULL == buffer)
    {
      // error!
      return EGE_ERROR_NO_MEMORY;
    }

    // read file data into buffer
    File file(path());
    if (EGE_SUCCESS != (result = file.open(EGEFile::MODE_READ_ONLY)))
    {
      // error!
      return result;
    }

    // get file size
    s64 size = file.size();

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
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IResource override. Unloads resource. */
void ResourceData::unload()
{
  m_data = NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

