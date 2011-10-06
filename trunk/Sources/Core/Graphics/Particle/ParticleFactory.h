#ifndef EGE_CORE_PARTICLEFACTORY_H
#define EGE_CORE_PARTICLEFACTORY_H

/** 
*   Factory (utility) class creating particle emitters and affectors. It is possible to register custom particle emitters/affectors so they can be created 
*   through factory.
*/

#include <EGE.h>
#include <EGEParticle.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DECLARE_SMART_CLASS(ParticleEmitter, PParticleEmitter)
EGE_DECLARE_SMART_CLASS(ParticleAffector, PParticleAffector)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

typedef PParticleEmitter  (*egeParticleEmitterCreateFunc)(Application* app, const String& name);
typedef PParticleAffector (*egeParticleAffectorCreateFunc)(Application* app, const String& name);

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class ParticleFactory : public Object
{
  public:

    ParticleFactory(Application* app);
   ~ParticleFactory();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Returns TRUE if object is valid. */
    bool isValid() const;

    /* Registeres custom particle emitter type. */
    EGEResult registerEmitter(const String& typeName, egeParticleEmitterCreateFunc createFunc);
    /* Creates instance of particle emitter of the type given by name. */
    PParticleEmitter createEmitter(const String& typeName, const String& name) const;
    /* Returns TRUE if given emitter type is registered. */
    bool isEmitterRegistered(const String& typeName) const;

    /* Registeres custom particle affector type. */
    EGEResult registerAffector(const String& typeName, egeParticleAffectorCreateFunc createFunc);
    /* Creates instance of particle affector of the type given by name. */
    PParticleAffector createAffector(const String& typeName, const String& name) const;
    /* Returns TRUE if given affector type is registered. */
    bool isAffectorRegistered(const String& typeName) const;

  private:

    /*! Class containing registration information for particle emitter. */
    class ParticleEmitterRegistryEntry
    {
      public:

        egeParticleEmitterCreateFunc m_createFunc;
    };

    /*! Class containing registration information for particle affector. */
    class ParticleAffectorRegistryEntry
    {
      public:

        egeParticleAffectorCreateFunc m_createFunc;
    };

  private:

    typedef Map<String, ParticleEmitterRegistryEntry> ParticleEmitterRegisterMap;
    typedef Map<String, ParticleAffectorRegistryEntry> ParticleAffectorRegisterMap;

  private:

    /*! Registered emitters sorted by type name. */
    ParticleEmitterRegisterMap m_registeredEmitters;
    /*! Registered affectors sorted by type name. */
    ParticleAffectorRegisterMap m_registeredAffectors;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_PARTICLEFACTORY_H
