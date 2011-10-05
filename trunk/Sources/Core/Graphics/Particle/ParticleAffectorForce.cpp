#include "Core/Graphics/Particle/ParticleAffectorForce.h"
#include <EGEGraphics.h>
#include <EGEDebug.h>

EGE_NAMESPACE

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
  //m_lifeSpan                    = params.value("life-span", "-1.0").toFloat(&error);
  //m_particleMaxCount            = params.value("max-particle-count", "100").toInt(&error);
  //m_emissionRate                = params.value("emission-rate", "15").toInt(&error);
  //m_particleStartSize           = params.value("particle-start-size", "10 10").toVector2f(&error);
  //m_particleStartSizeVariance   = params.value("particle-start-size-variance", "0 0").toVector2f(&error);
  //m_particleEndSize             = params.value("particle-end-size", "0 0").toVector2f(&error);
  //m_particleEndSizeVariance     = params.value("particle-end-size-variance", "0 0").toVector2f(&error);
  //m_particleLifeSpan            = params.value("particle-life-span", "5").toFloat(&error);
  //m_particleLifeSpanVariance    = params.value("particle-life-span-variance", "0").toFloat(&error);
  //m_particleStartColor          = params.value("particle-start-color", "0 0 0 1").toColor(&error);
  //m_particleStartColorVariance  = params.value("particle-start-color-variance", "0 0 0 0").toColor(&error);
  //m_particleEndColor            = params.value("particle-end-color", "1 1 1 1").toColor(&error);
  //m_particleEndColorVariance    = params.value("particle-end-color-variance", "0 0 0 0").toColor(&error);

  return !error;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
