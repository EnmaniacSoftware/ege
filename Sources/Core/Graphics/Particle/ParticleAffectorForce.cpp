#include "Core/Graphics/Particle/ParticleAffectorForce.h"
#include "EGEGraphics.h"
#include "EGEStringUtils.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ParticleAffectorForce)
EGE_DEFINE_DELETE_OPERATORS(ParticleAffectorForce)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ParticleAffectorForce::ParticleAffectorForce(const String& name) : ParticleAffector(name)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ParticleAffectorForce::~ParticleAffectorForce()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance of affector. This method is a registration method for factory. */
PParticleAffector ParticleAffectorForce::Create(const String& name)
{
  return ege_new ParticleAffectorForce(name);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! ParticleAffectorForce override. Initializes affector from dictionary. */
bool ParticleAffectorForce::initialize(const Dictionary& params)
{
  bool error = false;

  // decompose params
  m_force = StringUtils::ToVector3f(params.value("force", "0 0 0"), &error);

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