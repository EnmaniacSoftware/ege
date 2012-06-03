#include "Core/Graphics/Particle/ParticleAffectorForce.h"
#include <EGEGraphics.h>
#include <EGEDebug.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ParticleAffectorForce)
EGE_DEFINE_DELETE_OPERATORS(ParticleAffectorForce)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ParticleAffectorForce::ParticleAffectorForce(Application* app, const String& name) : ParticleAffector(app, name)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ParticleAffectorForce::~ParticleAffectorForce()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance of affector. This method is a registration method for factory. */
PParticleAffector ParticleAffectorForce::Create(Application* app, const String& name)
{
  return ege_new ParticleAffectorForce(app, name);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! ParticleAffectorForce override. Initializes affector from dictionary. */
bool ParticleAffectorForce::initialize(const Dictionary& params)
{
  bool error = false;

  // decompose params
  m_force = params.value("force", "0 0 0").toVector3f(&error);

  return !error;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! ParticleAffector override. Applies logic to given particles. 
 *  @param  time        Time increment for which calculations should be performed.
 *  @param  particles   Array of particles to apply data to.
 *  @param  count       Number of particles, counted from the first entry in array, for which calculations are to be done.
 */
void ParticleAffectorForce::apply(const Time& time, ParticleDataArray& particles, u32 count)
{
  // go thru all particles
  for (u32 i = 0; i < count; ++i)
  {
    EGEParticle::ParticleData& particleData = particles[i];
    
    // udate position
    particleData.position += m_force * time.seconds();    
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END