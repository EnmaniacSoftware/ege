#ifndef EGE_CORE_PARTICLEEMITTERPOINT_H
#define EGE_CORE_PARTICLEEMITTERPOINT_H

/** 
*   Particle emitter specalization. Such emitter generates all particles from area near its position.
*/

#include <EGE.h>
#include "Core/Graphics/Particle/ParticleEmitter.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(ParticleEmitterPoint, PParticleEmitterPoint)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ParticleEmitterPoint : public ParticleEmitter
{
  public:

    ParticleEmitterPoint(Application* app, const String& name);
    virtual ~ParticleEmitterPoint();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! Creates instance of emitter. This method is a registration method for factory. */
    static PParticleEmitter Create(Application* app, const String& name);

  public:

    /*! @see ParticleEmitter::initialize. */
    bool initialize(const Dictionary& params) override;

    /*! Sets emission angle. */
    void setEmissionAngle(const Angle& angle);
    /*! Sets emission angle variance. */
    void setEmissionAngleVariance(const Angle& variance);
    /*! Sets emitter direction. */
    void setEmissionDirection(const Vector3f& direction);
    /*! Sets emitter direction mask. */
    void setEmissionDirectionMask(const Vector3f& directionMask);

    /*! Sets particle speed. */
    void setParticleSpeed(float32 speed);
    /*! Sets particle speed variance. */
    void setParticleSpeedVariance(float32 variance);
    /*! Sets particle start position variance. */
    void setParticleStartPositionVariance(const Vector3f& variance);
    /*! Sets emission acceleration. */
    void setParticleAcceleration(const Vector3f& acceleration);
    /*! Sets emission acceleration variance. */
    void setParticleAccelerationVariance(const Vector3f& variance);

  private:

    /*! @see ParticleEmitter::initializeParticle. */
    void initializeParticle(s32 index) override;

  private:

    /*! Particle emission angle. */
    Angle m_emissionAngle;
    /*! Particle emission angle variance. */
    Angle m_emissionAngleVariance;
    /*! Emission direction. */
    Vector3f m_emissionDirection;
    /*! Emission direction mask. Mask allows to restrict direction of emission if required. */
    Vector3f m_emissionDirectionMask;
    /*! Particles start position variance. */
    Vector3f m_particleStartPositionVariance;
    /*! Particle speed. */
    float32 m_particleSpeed;
    /*! Particle speed variance. */
    float32 m_particleSpeedVariance;
    /*! Emission acceleration vector. */
    Vector3f m_emissionAcceleration;
    /*! Emission acceleration vector variance. */
    Vector3f m_emissionAccelerationVariance;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_PARTICLEEMITTERPOINT_H
