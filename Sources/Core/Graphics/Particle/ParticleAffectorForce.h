#ifndef EGE_CORE_PARTICLEAFFECTORFORCE_H
#define EGE_CORE_PARTICLEAFFECTORFORCE_H

/** 
*   Particle affector specialization. Affector changes the position of particles according to force direction and magnitude.
*/

#include "EGE.h"
#include "EGEParticle.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(ParticleAffectorForce, PParticleAffectorForce)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ParticleAffectorForce : public ParticleAffector
{
  public:

    ParticleAffectorForce(const String& name);
    virtual ~ParticleAffectorForce();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! Creates instance of affector. This method is a registration method for factory. */
    static PParticleAffector Create(const String& name);

  public:

    /*! @see ParticleAffector::initialize. */
    bool initialize(const Dictionary& params) override;
    /*! @see ParticleAffector::apply. */
    void apply(const Time& time, ParticleDataArray& particles, u32 count) override;

  private:

    /*! Force vector. */
    Vector3f m_force;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_PARTICLEAFFECTORFORCE_H
