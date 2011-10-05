#include "Core/Graphics/Particle/ParticleFactory.h"
#include <EGEDebug.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(ParticleFactory)
EGE_DEFINE_DELETE_OPERATORS(ParticleFactory)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
struct BuiltInEmitter
{
  const char* name;
  egeParticleEmitterCreateFunc pfCreateFunc;
};

static BuiltInEmitter l_emittersToRegister[] = {  { "point", ParticleEmitterPoint::Create }
};

struct BuiltInAffector
{
  const char* name;
  egeParticleAffectorCreateFunc pfCreateFunc;
};

static BuiltInAffector l_affectorsToRegister[] = {  { "force", ParticleAffectorForce::Create }
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ParticleFactory::ParticleFactory(Application* app) : Object(app)
{
  // register build-in emitter types
  for (u32 i = 0; i < sizeof (l_emittersToRegister) / sizeof (BuiltInEmitter); ++i)
  {
    const BuiltInEmitter& emitter = l_emittersToRegister[i];

    registerEmitter(emitter.name, emitter.pfCreateFunc);
  }

  // register build-in affector types
  for (u32 i = 0; i < sizeof (l_affectorsToRegister) / sizeof (BuiltInAffector); ++i)
  {
    const BuiltInAffector& affector = l_affectorsToRegister[i];

    registerAffector(affector.name, affector.pfCreateFunc);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ParticleFactory::~ParticleFactory()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool ParticleFactory::isValid() const
{
  // check if built-in emitters are registered correctly
  for (u32 i = 0; i < sizeof (l_emittersToRegister) / sizeof (BuiltInEmitter); ++i)
  {
    const BuiltInEmitter& emitter = l_emittersToRegister[i];

    if (!isEmitterRegistered(emitter.name))
    {
      // error!
      return false;
    }
  }

  // check if built-in affectors are registered correctly
  for (u32 i = 0; i < sizeof (l_affectorsToRegister) / sizeof (BuiltInAffector); ++i)
  {
    const BuiltInAffector& affector = l_affectorsToRegister[i];

    if (!isAffectorRegistered(affector.name))
    {
      // error!
      return false;
    }
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Registeres custom resource type. */
EGEResult ParticleFactory::registerEmitter(const String& typeName, egeParticleEmitterCreateFunc createFunc)
{
  EGEResult result = EGE_SUCCESS;
  
  // check if resource with such a name exists already
  if (isEmitterRegistered(typeName))
  {
    // error!
    return EGE_ERROR_ALREADY_EXISTS;
  }

  // register
  ParticleEmitterRegistryEntry entry;
  entry.m_createFunc  = createFunc;

  m_registeredEmitters.insert(typeName, entry);

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance of particle emitter of the type given by name. */
PParticleEmitter ParticleFactory::createEmitter(const String& typeName, const String& name) const
{
  PParticleEmitter emitter;

  // file emitter with given type name
  ParticleEmitterRegisterMap::const_iterator it = m_registeredEmitters.find(typeName);
  if (it != m_registeredEmitters.end())
  {
    // create resource
    emitter = it->second.m_createFunc(app(), name);
  }

  return emitter;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if given emitter type is registered. */
bool ParticleFactory::isEmitterRegistered(const String& typeName) const
{
  return m_registeredEmitters.contains(typeName);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Registeres custom particle affector type. */
EGEResult ParticleFactory::registerAffector(const String& typeName, egeParticleAffectorCreateFunc createFunc)
{
  EGEResult result = EGE_SUCCESS;
  
  // check if resource with such a name exists already
  if (isAffectorRegistered(typeName))
  {
    // error!
    return EGE_ERROR_ALREADY_EXISTS;
  }

  // register
  ParticleAffectorRegistryEntry entry;
  entry.m_createFunc  = createFunc;

  m_registeredAffectors.insert(typeName, entry);

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance of particle affector of the type given by name. */
PParticleAffector ParticleFactory::createAffector(const String& typeName, const String& name) const
{
  PParticleAffector affector;

  // file emitter with given type name
  ParticleAffectorRegisterMap::const_iterator it = m_registeredAffectors.find(typeName);
  if (it != m_registeredAffectors.end())
  {
    // create resource
    affector = it->second.m_createFunc(app(), name);
  }

  return affector;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if given affector type is registered. */
bool ParticleFactory::isAffectorRegistered(const String& typeName) const
{
  return m_registeredAffectors.contains(typeName);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
