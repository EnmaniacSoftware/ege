#include "Core/Graphics/Particle/ParticleAffector.h"
#include <EGEGraphics.h>
#include <EGEDebug.h>

EGE_NAMESPACE

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
/*! Initializes affector from dictionary. */
bool ParticleAffector::initialize(const Dictionary& params)
{
  bool error = false;

  return !error;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
