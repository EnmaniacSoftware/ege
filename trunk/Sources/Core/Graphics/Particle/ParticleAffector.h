#ifndef EGE_CORE_PARTICLEAFFECTOR_H
#define EGE_CORE_PARTICLEAFFECTOR_H

/** 
*   This is base object for specialization of different particle affectors. Particle affector is an object responsible for modification of spawn particles
*   according to predefined data.
*/

#include <EGE.h>
#include <EGETime.h>
#include <EGEString.h>
#include <EGEDictionary.h>
#include <EGEParticle.h>
#include <EGESignal.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DECLARE_SMART_CLASS(ParticleAffector, PParticleAffector)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class ParticleAffector : public Object
{
  friend class ParticleEmitter;

  public:

    ParticleAffector(Application* app, const String& name);
    virtual ~ParticleAffector();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /* Initializes affector from dictionary. */
    virtual bool initialize(const Dictionary& params);
    /*! Applies logic to given particles. 
     *  @param  time        Time increment for which calculations should be performed.
     *  @param  particles   Array of particles to apply data to.
     *  @param  count       Number of particles, counted from the first entry in array, for which calculations are to be done.
     */
    virtual void apply(const Time& time, ParticleDataArray& particles, u32 count) = 0;
    
  protected:

    /*! Returns pointer to parent emitter. */
    inline ParticleEmitter* emitter() const { return m_emitter; }

  private:

    /* Returns TRUE if detailed particle information is required like notification about particle spawns/deaths. */
    virtual bool detailedParticleInfoRequired() const;

  private slots:

    /* Slot called when new particle has been spawned. */
    virtual void onNewParticleSpawned(const EGEParticle::ParticleData& particle);
    /* Slot called when particle dies. */
    virtual void onParticleDied(const EGEParticle::ParticleData& particle, s32 index);

  protected:

    /*! Active flag. */
    bool m_active;

  private:

    /*! Emitter affector is attached to. NULL if not attached to any emitter. Only settable by ParticleEmitter. */
    ParticleEmitter* m_emitter;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_PARTICLEAFFECTOR_H
