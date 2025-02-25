#ifndef EGE_CORE_PARTICLEEMITTER_H
#define EGE_CORE_PARTICLEEMITTER_H

/** 
*   This is base object for specialization of different particle emitters. Particle emitter is an object responsible for creation and managing the particles 
*   according to predefined data.
*/

#include "EGE.h"
#include "EGEVertexBuffer.h"
#include "EGEDynamicArray.h"
#include "EGEList.h"
#include "EGETime.h"
#include "EGEColor.h"
#include "EGEVector2.h"
#include "EGEScene.h"
#include "EGEParticle.h"
#include "EGESignal.h"
#include "EGEMaterial.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(ParticleEmitter, PParticleEmitter)
EGE_DECLARE_SMART_CLASS(ParticleAffector, PParticleAffector)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ParticleEmitter : public SceneNodeObject
{
  public:

    ParticleEmitter(Application* app, const String& name);
    virtual ~ParticleEmitter();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! Signal emitted when new particle has been spawned. 
     *  @param  particleData  Data of particle which has been just spawned.
     *  @note   Signal is emitted just after particle has been added into particle pool. Thus, it is guaranteed it will be found at the position of last active 
     *          particle.
     */
    Signal1<const EGEParticle::ParticleData&> particleSpawned;
    /*! Signal emitted when particle dies.
     *  @param  particleData  Data of particle which has just died.
     *  @param  index         Index of particle within particle pool.
     *  @note   Signal is emitted just before particle is removed from pool. Thus, it is guaranteed index will point to proper entry in the pool.
     */
    Signal2<const EGEParticle::ParticleData&, s32> particleDied;

  public:

    /*! Initializes emitter from dictionary. */
    virtual bool initialize(const Dictionary& params);
    /*! Starts system. 
     *  @note This resets all data. 
     */
    void start();
    /*! Stops system. */
    void stop();
    /*! Updates object. */
    void update(const Time& time);
    /*! Returns TRUE if emitter is working. */
    bool isRunning() const { return m_active; }

    /*! Sets system life span. Negative time causes emitter to live infinitely. */
    void setLifeSpan(const Time& time);
    /*! Sets maximum number of particles. */
    void setParticleMaxCount(s32 count);
    /*! Sets emission rate. */
    void setEmissionRate(s32 rate);
    /*! Returns number of active particles. */
    s32 activeParticlesCount() const { return m_activeParticlesCount; }

    /*! Sets particle start size. */
    void setParticleStartSize(const Vector2f& size);
    /*! Sets particle start size variance. */
    void setParticleStartSizeVariance(const Vector2f& variance);
    /*! Sets particle end size. */
    void setParticleEndSize(const Vector2f& size);
    /*! Sets particle end size variance. */
    void setParticleStartEndVariance(const Vector2f& variance);
    /*! Sets particle life span. */
    void setParticleLifeSpan(const Time& duration);
    /*! Sets particle life span variance. */
    void setParticleLifeSpanVariance(const Time& variance);
    /*! Sets particle start color. */
    void setParticleStartColor(const Color& color);
    /*! Sets particle start color variance. */
    void setParticleStartColorVariance(const Color& variance);
    /*! Sets particle end color. */
    void setParticleEndColor(const Color& color);
    /*! Sets particle end color variance. */
    void setParticleEndColorVariance(const Color& variance);
    /*! Sets particle spin speed (degs/sec). */
    void setParticleSpinSpeed(float32 speed);
    /*! Sets particle spin speed variance (degs/sec). */
    void setParticleSpinSpeedVariance(float32 variance);
   
    /*! Sets material. */
    void setMaterial(const PMaterial& material);

    /*! Adds affector. */
    void addAffector(PParticleAffector& affector);
    /*! Removes given affector. */
    void removeAffector(PParticleAffector& affector);

    /*! Returns render component. */
    const PRenderComponent& renderComponent() const { return m_renderData; }

    /*! Adds object render data for rendering with given renderer. */
    bool addForRendering(IRenderer* renderer, const Matrix4f& transform = Matrix4f::IDENTITY) override;

  private:

    /*! Returns TRUE if there is no available space for new particle. */
    bool isFull() const { return m_activeParticlesCount == m_particleMaxCount; }
    /*! Allocates particles data. */
    bool allocateParticlesData();
    /*! Applies affectors. */
    void applyAffectors(const Time& time);
    /*! Initializes particle at given index. */
    virtual void initializeParticle(s32 index) = 0;

  protected:

    typedef List<PParticleAffector> ParticleAffectorList;

  protected:

    /*! Active flag. */
    bool m_active;
    /*! Life span. */
    Time m_lifeSpan;
    /*! Current life duration. */
    Time m_lifeDuration;
    /*! Maximum number of particles. */
    s32 m_particleMaxCount;
    /*! Current number of active particles. */
    s32 m_activeParticlesCount;
    /*! Emission rate (particles/sec). */
    s32 m_emissionRate;
    /*! Number of particles to emit. This can be fractional if it is still to early to emit next particle. */
    float32 m_emitCount;
    /*! TRUE if particles should spawn in local space. */
    bool m_localSpace;
    /*! Particles start size. */
    Vector2f m_particleStartSize;
    /*! Particles start size variance. */
    Vector2f m_particleStartSizeVariance;
    /*! Particles end size. */
    Vector2f m_particleEndSize;
    /*! Particles end size variance. */
    Vector2f m_particleEndSizeVariance;
    /*! Particle life span. */
    Time m_particleLifeSpan;
    /*! Particle life span variance. */
    Time m_particleLifeSpanVariance;
    /*! Particle start color. */
    Color m_particleStartColor;
    /*! Particle start color variance. */
    Color m_particleStartColorVariance;
    /*! Particle end color. */
    Color m_particleEndColor;
    /*! Particle end color variance. */
    Color m_particleEndColorVariance;
    /*! Particle spin speed (degs/sec). */
    float32 m_particleSpinSpeed;
    /*! Particle spin speed variance (degs/sec). */
    float32 m_particleSpinSpeedVariance;
    /*! Render data. */
    PRenderComponent m_renderData;
    /*! Array of particles. */
    ParticleDataArray m_particles;
    /*! List of affectors. */
    ParticleAffectorList m_affectors;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_PARTICLEEMITTER_H
