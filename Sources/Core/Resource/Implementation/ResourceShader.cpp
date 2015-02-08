#include "Core/Resource/Interface/ResourceShader.h"
#include "Core/Graphics/Graphics.h"
#include "Core/Graphics/HardwareResourceProvider.h"
#include "Core/Graphics/Render/RenderSystem.h"
#include "EGEEngine.h"
#include "EGEFileUtils.h"
#include "EGEGraphics.h"
#include "EGEXml.h"
#include "EGEResources.h"
#include "EGEStringUtils.h"
#include "EGEDataBuffer.h"
#include "EGEFile.h"
#include "EGEDebug.h"
#include "EGEDirectory.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const char* KResourceShaderDebugName = "EGEResourceShader";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ResourceShader)
EGE_DEFINE_DELETE_OPERATORS(ResourceShader)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function mapping literal shader type name into numeric value. */
static EGEGraphics::ShaderType MapShaderTypeName(const String& name)
{
  if ("vertex" == name)
  {
    return EGEGraphics::VERTEX_SHADER;
  }
  else if ("fragment" == name)
  {
    return EGEGraphics::FRAGMENT_SHADER;
  }

  return EGEGraphics::UNKNOWN_SHADER;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceShader::ResourceShader(Engine& engine, ResourceGroup* group) 
: IResource(engine, group, RESOURCE_NAME_SHADER)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceShader::~ResourceShader()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const String& ResourceShader::name() const
{
  return m_name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceShader::create(const String& path, const PObject& data)
{
  EGEResult result = EGE_SUCCESS;

  PXmlElement tag = ege_pcast<PXmlElement>(data);

  bool error = false;

  // get data
  m_name = tag->attribute("name");
  m_path = tag->attribute("path");
  m_type = MapShaderTypeName(tag->attribute("type").toLower());

  // check if obligatory data is wrong
  if (m_name.isEmpty() || m_path.isEmpty() || (EGEGraphics::UNKNOWN_SHADER == m_type) || error)
  {
    // error!
    egeWarning(KResourceShaderDebugName) << "Failed for name:" << m_name;
    return EGE_ERROR_BAD_PARAM;
  }

  // compose absolute path
  m_path = Directory::Join(path, m_path);

  // set state
  m_state = STATE_UNLOADED;

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceShader::load()
{
  EGEResult result = EGE_SUCCESS;

  if (STATE_UNLOADED == m_state)
  {
    // create
    result = create();
    if (EGE_SUCCESS == result)
    {
      // set to loading
      m_state = STATE_LOADING;
    }
  }
  
  // check if still loading
  if (STATE_LOADING == m_state)
  {
    result = EGE_WAIT;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceShader::create()
{
  EGEResult result;

  // allocate buffer
  m_data = ege_new DataBuffer();
  if (NULL == m_data)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  // read shader file into buffer
  File file(path());
  if (EGE_SUCCESS != (result = file.open(EFileModeReadOnly)))
  {
    // error!
    return result;
  }

  const s64 fileSize = FileUtils::Size(file.filePath());
  if (file.read(m_data, fileSize) != fileSize)
  {
    // error!
    result = EGE_ERROR_IO;
  }

  file.close();

  // request texture
  if ((EGE_SUCCESS == result) && ! engine().graphics()->hardwareResourceProvider()->requestCreateShader(m_type, name(), m_data,
                                                                                                        ege_make_slot(this, ResourceShader::onRequestComplete)))
  {
    // error!
    result = EGE_ERROR;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceShader::unload() 
{ 
  if (STATE_LOADED == m_state)
  {
    // schedule for destroy
    engine().graphics()->hardwareResourceProvider()->requestDestroyShader(m_shader);

    // clean up
    m_shader = NULL; 

    // reset flag
    m_state = STATE_UNLOADED;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceShader::onRequestComplete(PObject object)
{
  // store handle
  m_shader = object;

  // set state
  m_state = STATE_LOADED;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END