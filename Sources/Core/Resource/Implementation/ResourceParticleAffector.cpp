#include "Core/Resource/Interface/ResourceParticleAffector.h"
#include "EGEEngine.h"
#include "EGEGraphics.h"
#include "EGEXml.h"
#include "EGEDebug.h"
#include "EGEResources.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const char* KResourceParticleAffectorDebugName = "EGEResourceParticleAffector";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ResourceParticleAffector)
EGE_DEFINE_DELETE_OPERATORS(ResourceParticleAffector)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceParticleAffector::ResourceParticleAffector(Engine& engine, ResourceGroup* group) 
: IResource(engine, group, RESOURCE_NAME_PARTICLE_AFFECTOR)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceParticleAffector::~ResourceParticleAffector()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PResource ResourceParticleAffector::Create(Engine& engine, ResourceGroup* group)
{
  return ege_new ResourceParticleAffector(engine, group);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const String& ResourceParticleAffector::name() const
{
  return m_name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceParticleAffector::create(const String& path, const PObject& data)
{
  EGE_UNUSED(path);

  EGEResult result = EGE_SUCCESS;

  PXmlElement tag = ege_pcast<PXmlElement>(data);

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
    egeDebug(KResourceParticleAffectorDebugName) << "Failed for name:" << m_name;
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
  PParticleAffector object = engine().graphics()->particleFactory()->createAffector(m_parameters["type"], m_name);
  if (object)
  {
    // initialize with dictionary
    if (!object->initialize(m_parameters))
    {
      // error!
      egeWarning(KResourceParticleAffectorDebugName) << "Could not initialize!";
      object = NULL;
    }
  }

  return object;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END