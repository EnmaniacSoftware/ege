#include "Core/Graphics/Particle/ParticleFactory.h"
#include "Core/Graphics/Particle/ParticleEmitterPoint.h"
#include "Core/Graphics/Particle/ParticleAffectorForce.h"
#include <EGEDebug.h>

EGE_NAMESPACE_BEGIN

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
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ParticleFactory::~ParticleFactory()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ParticleFactory::construct()
{
  // register build-in emitter types
  for (u32 i = 0; i < sizeof (l_emittersToRegister) / sizeof (BuiltInEmitter); ++i)
  {
    const BuiltInEmitter& emitter = l_emittersToRegister[i];

    if ( ! registerEmitter(emitter.name, emitter.pfCreateFunc))
    {
      // error!
      return EGE_ERROR;
    }
  }

  // register build-in affector types
  for (u32 i = 0; i < sizeof (l_affectorsToRegister) / sizeof (BuiltInAffector); ++i)
  {
    const BuiltInAffector& affector = l_affectorsToRegister[i];

    if ( ! registerAffector(affector.name, affector.pfCreateFunc))
    {
      // error!
      return EGE_ERROR;
    }
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
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
bool ParticleFactory::isEmitterRegistered(const String& typeName) const
{
  return m_registeredEmitters.contains(typeName);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
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
bool ParticleFactory::isAffectorRegistered(const String& typeName) const
{
  return m_registeredAffectors.contains(typeName);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END