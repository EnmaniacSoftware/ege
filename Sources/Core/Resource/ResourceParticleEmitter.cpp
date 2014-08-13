#include "Core/Resource/ResourceParticleEmitter.h"
#include "Core/Resource/ResourceManager.h"
#include "EGEEngine.h"
#include "EGEGraphics.h"
#include "EGEXml.h"
#include "EGEDebug.h"
#include "EGEResources.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const char* KResourceParticleEmitterDebugName = "EGEResourceParticleEmitter";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define NODE_AFFECTOR "affector"
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ResourceParticleEmitter)
EGE_DEFINE_DELETE_OPERATORS(ResourceParticleEmitter)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceParticleEmitter::ResourceParticleEmitter(Engine& engine, ResourceGroup* group) : IResource(engine, group, RESOURCE_NAME_PARTICLE_EMITTER)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceParticleEmitter::~ResourceParticleEmitter()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PResource ResourceParticleEmitter::Create(Engine& engine, ResourceGroup* group)
{
  return ege_new ResourceParticleEmitter(engine, group);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const String& ResourceParticleEmitter::name() const
{
  return m_name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceParticleEmitter::create(const String& path, const PXmlElement& tag)
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
  if (!m_parameters.contains("name") || !m_parameters.contains("type") || !m_parameters.contains("material"))
  {
    // error!
    egeWarning(KResourceParticleEmitterDebugName) << "Failed for name:" << m_name;
    return EGE_ERROR_BAD_PARAM;
  }

  // store name separately for further use
  m_name = m_parameters["name"];

  // go thru all sub node
  PXmlElement child = tag->firstChild();
  while (child->isValid())
  {
    // check child
    if (NODE_AFFECTOR == child->name())
    {
      // add affector
      result = addAffector(child);
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

  // check if success
  if (EGE_SUCCESS == result)
  {
    // set state
    m_state = STATE_UNLOADED;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceParticleEmitter::load()
{
  EGEResult result = EGE_SUCCESS;

  if (STATE_LOADED != m_state)
  {
    // get material resource
    m_materialResource = group()->manager()->resource(RESOURCE_NAME_MATERIAL, m_parameters["material"]);
    if (m_materialResource)
    {
      // load material
      if (EGE_SUCCESS != (result = m_materialResource->load()))
      {
        // error!
        m_materialResource = NULL;
        return result;
      }
    }
    else
    {
      // material not found
      result = EGE_ERROR_NOT_FOUND;
    }

    // load all affectors
    for (StringList::const_iterator it = m_affectors.begin(); it != m_affectors.end(); ++it)
    {
      // get affector resource
      PResourceParticleAffector affectorResource = group()->manager()->resource(RESOURCE_NAME_PARTICLE_AFFECTOR, *it);
      if (affectorResource)
      {
        // load it
        if (EGE_SUCCESS != (result = affectorResource->load()))
        {
          // error!
          egeWarning(KResourceParticleEmitterDebugName) << "Could not load!";
          return result;
        }
      }
      else
      {
        // affector not found
        result = EGE_ERROR_NOT_FOUND;
      }
    }

    // set flag
    m_state = STATE_LOADED;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceParticleEmitter::unload() 
{ 
  // clean up
  m_materialResource = NULL;

  // reset flag
  m_state = STATE_UNLOADED;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PParticleEmitter ResourceParticleEmitter::createInstance()
{
  // create instance of particle emitter of a correct type and with given name
  PParticleEmitter object = engine().graphics()->particleFactory()->createEmitter(m_parameters["type"], m_name);
  if (object)
  {
    // initialize with dictionary
    if (!object->initialize(m_parameters))
    {
      // error!
      egeWarning(KResourceParticleEmitterDebugName) << "Could not initialize!";
      object = NULL;
    }

    // add affectors
    for (StringList::const_iterator it = m_affectors.begin(); it != m_affectors.end(); ++it)
    {
      // get affector resource
      PResourceParticleAffector affectorResource = group()->manager()->resource(RESOURCE_NAME_PARTICLE_AFFECTOR, *it);
      EGE_ASSERT(affectorResource);

      // create instance
      PParticleAffector affector = affectorResource->createInstance();

      // add to emitter
      object->addAffector(affector);
    }

    // assign material
    object->setMaterial(m_materialResource->createInstance());
  }

  return object;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ResourceParticleEmitter::addAffector(const PXmlElement& tag)
{
  // get obligatory data
  String name = tag->attribute("name");
  if (name.empty())
  {
    // error!
    return EGE_ERROR;
  }

  m_affectors.push_back(name);
  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END