#include "Core/Graphics/Particle/ParticleAffector.h"
#include <EGEGraphics.h>
#include <EGEDebug.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ParticleAffector)
EGE_DEFINE_DELETE_OPERATORS(ParticleAffector)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ParticleAffector::ParticleAffector(Application* app, const String& name) : Object(app)
{
  // initialize to default values
  Dictionary params;
  initialize(params);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ParticleAffector::~ParticleAffector()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ParticleAffector::initialize(const Dictionary& params)
{
  bool error = false;

  return !error;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ParticleAffector::onNewParticleSpawned(const EGEParticle::ParticleData& particle)
{
  EGE_UNUSED(particle);

  // do nothing here
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ParticleAffector::onParticleDied(const EGEParticle::ParticleData& particle, s32 index)
{
  EGE_UNUSED(particle);

  // do nothing here
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ParticleAffector::detailedParticleInfoRequired() const
{
  // by default, affectors dont expect this info
  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END