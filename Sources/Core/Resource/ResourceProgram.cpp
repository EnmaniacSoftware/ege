#include "Core/Resource/ResourceProgram.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Resource/ResourceShader.h"
#include "Core/Graphics/Graphics.h"
#include "Core/Graphics/HardwareResourceProvider.h"
#include "Core/Graphics/Render/RenderSystem.h"
#include "EGEApplication.h"
#include "EGEGraphics.h"
#include "EGEResources.h"
#include "EGEShader.h"
#include "EGEList.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const char* KResourceProgramDebugName = "EGEResourceProgram";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define NODE_SHADER_REF "shader-ref"
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ResourceProgram)
EGE_DEFINE_DELETE_OPERATORS(ResourceProgram)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceProgram::ResourceProgram(Application* app, ResourceGroup* group) : IResource(app, group, RESOURCE_NAME_PROGRAM)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceProgram::~ResourceProgram()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PResource ResourceProgram::Create(Application* app, ResourceGroup* group)
{
  return ege_new ResourceProgram(app, group);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const String& ResourceProgram::name() const
{
  return m_name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceProgram::create(const String& path, const PXmlElement& tag)
{
  EGEResult result = EGE_SUCCESS;

  // get data
  m_name = tag->attribute("name");

  // check if obligatory data is wrong
  if (m_name.empty())
  {
    // error!
    egeWarning(KResourceProgramDebugName) << "Failed for name:" << m_name;
    return EGE_ERROR_BAD_PARAM;
  }

  // go thru all sub node
  PXmlElement child = tag->firstChild();
  while (child->isValid())
  {
    // check child
    if (NODE_SHADER_REF == child->name())
    {
      // add shader reference
      result = addShaderReference(child);
    }

    // check if failed
    if (EGE_SUCCESS != result)
    {
      // error, done!
      break;
    }

    // go to next child
    child = child->nextChild();
  }

  // set state
  m_state = STATE_UNLOADED;

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceProgram::load()
{
  EGEResult result = EGE_SUCCESS;

  if (STATE_UNLOADED == m_state)
  {
    // try to load all dependencies
    result = loadDependencies();
    if (EGE_SUCCESS == result)
    {
      // request program if necessary
      if (STATE_LOADING != m_state)
      {
        List<PShader> shaders;
        for (ShaderMap::const_iterator it = m_shaders.begin(); it != m_shaders.end(); ++it)
        {
          shaders.push_back(it->second);
        }

        // request program
        app()->graphics()->hardwareResourceProvider()->requestCreateProgram(name(), shaders, ege_make_slot(this, ResourceProgram::onRequestComplete));

        // set state
        m_state = STATE_LOADING;
      }
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
void ResourceProgram::unload() 
{ 
  if (STATE_LOADED == m_state)
  {
    // unload shaders
    for (ShaderMap::iterator it = m_shaders.begin(); it != m_shaders.end(); ++it)
    {
      const String& name = it->first;
      PResourceShader resource = group()->manager()->resource(RESOURCE_NAME_SHADER, name);
      if (NULL != resource)
      {
        resource->unload();
      }

      it->second = NULL;
    }

    // schedule for destroy
    app()->graphics()->hardwareResourceProvider()->requestDestroyProgram(m_program);

    // clean up
    m_program = NULL; 

    // reset flag
    m_state = STATE_UNLOADED;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceProgram::onRequestComplete(PObject object)
{
  // store handle
  m_program = object;

  // set state
  m_state = STATE_LOADED;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceProgram::addShaderReference(const PXmlElement& tag)
{
  // get data
  String name = tag->attribute("name");
  if (name.empty())
  {
    // error!
    return EGE_ERROR_NOT_FOUND;
  }

  // add to pool
  m_shaders.insert(name, NULL);

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceProgram::loadDependencies()
{
  // try to load (if necessary) shaders
  for (ShaderMap::iterator it = m_shaders.begin(); it != m_shaders.end(); ++it)
  {
    const String& name = it->first;

    // check if loaded
    if (NULL != it->second)
    {
      // skip
      continue;
    }

    // try to find shader resource of a given name
    PResourceShader resource = group()->manager()->resource(RESOURCE_NAME_SHADER, name);
    if (NULL == resource)
    {
      // error!
      return EGE_ERROR_NOT_FOUND;
    }

    // load it
    EGEResult result = resource->load();
    if (EGE_SUCCESS != result)
    {
      return result;
    }

    // store it
    it->second = resource->shader();
  }

  // done
  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END