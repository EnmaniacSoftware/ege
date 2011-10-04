#ifndef EGE_CORE_PARTICLEEMITTERFACTORY_H
#define EGE_CORE_PARTICLEEMITTERFACTORY_H

/** 
*   Factory (utility) class creating particle emitters. It is possible to register custom particle emitter so they can be created thright factory.
*/

#include <EGE.h>
#include <EGEParticle.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class ParticleEmitterFactory
{
  public:

    ParticleEmitterFactory();
   ~ParticleEmitterFactory();
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_PARTICLEEMITTERFACTORY_H
