#include "Core/Resource/ResourceShader.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Graphics/Graphics.h"
#include "Core/Graphics/HardwareResourceProvider.h"
#include "Core/Graphics/Render/RenderSystem.h"
#include <EGEApplication.h>
#include <EGEGraphics.h>
#include <EGEXml.h>
#include <EGEResources.h>
#include <EGEStringUtils.h>
#include <EGEDebug.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ResourceShader)
EGE_DEFINE_DELETE_OPERATORS(ResourceShader)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceShader::ResourceShader(Application* app, ResourceGroup* group) : IResource(app, group, RESOURCE_NAME_SHADER),
                                                                         m_resourceRequestId(0)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceShader::~ResourceShader()
{
  if ((NULL != app()->graphics()) && (NULL != app()->graphics()->hardwareResourceProvider()))
  {
    ege_disconnect(app()->graphics()->hardwareResourceProvider(), requestComplete, this, ResourceShader::onRequestComplete);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PResource ResourceShader::Create(Application* app, ResourceGroup* group)
{
  return ege_new ResourceShader(app, group);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const String& ResourceShader::name() const
{
  return m_name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceShader::create(const String& path, const PXmlElement& tag)
{
  EGEResult result = EGE_SUCCESS;

  bool error = false;

  // get data
  m_name            = tag->attribute("name");
  m_path            = tag->attribute("path");
  m_type            = tag->attribute("type").toLower();

  // check if obligatory data is wrong
  if (m_name.empty() || m_path.empty() || m_type.empty() || error)
  {
    // error!
    egeWarning() << "Failed for name:" << m_name;
    return EGE_ERROR_BAD_PARAM;
  }

  // compose absolute path
  m_path = path + "/" + m_path;

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
  //// load image data
  //PImage image = Image::Load(path());
  //if (NULL == image)
  //{
  //  // error!
  //  return EGE_ERROR;
  //}

  //// request texture
  //m_resourceRequestId = app()->graphics()->hardwareResourceProvider()->requestCreateShader(name(), image);

  // connect for notification
  ege_connect(app()->graphics()->hardwareResourceProvider(), requestComplete, this, ResourceShader::onRequestComplete);

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceShader::unload() 
{ 
  if (STATE_LOADED == m_state)
  {
    app()->graphics()->hardwareResourceProvider()->requestDestroyShader(m_shader);

    // clean up
    m_shader = NULL; 

    // reset flag
    m_state = STATE_UNLOADED;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceShader::onRequestComplete(u32 handle, PObject object)
{
  if (handle == m_resourceRequestId)
  {
    // store handle
    m_shader = object;

    // disconnect
    ege_disconnect(app()->graphics()->hardwareResourceProvider(), requestComplete, this, ResourceShader::onRequestComplete);
    m_resourceRequestId = 0;

    // set state
    m_state = STATE_LOADED;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END