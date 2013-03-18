#include "Core/Resource/ResourceParticleAffector.h"
#include "Core/Resource/ResourceManager.h"
#include "EGEApplication.h"
#include "EGEGraphics.h"
#include "EGEXml.h"
#include "EGEDebug.h"
#include "EGEResources.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ResourceParticleAffector)
EGE_DEFINE_DELETE_OPERATORS(ResourceParticleAffector)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceParticleAffector::ResourceParticleAffector(Application* app, ResourceGroup* group) : IResource(app, group, RESOURCE_NAME_PARTICLE_AFFECTOR)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceParticleAffector::~ResourceParticleAffector()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PResource ResourceParticleAffector::Create(Application* app, ResourceGroup* group)
{
  return ege_new ResourceParticleAffector(app, group);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const String& ResourceParticleAffector::name() const
{
  return m_name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceParticleAffector::create(const String& path, const PXmlElement& tag)
{
  EGE_UNUSED(path);

  EGEResult result = EGE_SUCCESS;

  // parse all attributes
  PXmlAttribute attribute = tag->firstAttribute();
  while (attribute)
  {
    // store data
    m_parameters[attribute->name()] = attribute->value();

    // go to next attribute
    attribute = attribute->next();
  }

  // check if obligatory data is wrong
  if (!m_parameters.contains("name") || !m_parameters.contains("type"))
  {
    // error!
    egeDebug() << "Failed for name:" << m_name;
    return EGE_ERROR_BAD_PARAM;
  }

  // store name separately for further use
  m_name = m_parameters["name"];

  // check if success
  if (EGE_SUCCESS == result)
  {
    // set state
    m_state = STATE_UNLOADED;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceParticleAffector::load()
{
  // set flag
  m_state = STATE_LOADED;

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceParticleAffector::unload() 
{ 
  // reset flag
  m_state = STATE_UNLOADED;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PParticleAffector ResourceParticleAffector::createInstance()
{
  // create instance of particle emitter of a correct type and with given name
  PParticleAffector object = app()->graphics()->particleFactory()->createAffector(m_parameters["type"], m_name);
  if (object)
  {
    // initialize with dictionary
    if (!object->initialize(m_parameters))
    {
      // error!
      egeWarning() << "Could not initialize!";
      object = NULL;
    }
  }

  return object;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END