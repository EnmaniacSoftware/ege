#ifndef EGE_CORE_PARTICLEEMITTER_H
#define EGE_CORE_PARTICLEEMITTER_H

/** 
*   This is base object for specialization of different particle emitters. Particle emitter is an object responsible for creation and managing the particles 
*   according to predefined data.
*/

#include <EGE.h>
#include <EGEVertexBuffer.h>
#include <EGEDynamicArray.h>
#include <EGEList.h>
#include <EGETime.h>
#include <EGEColor.h>
#include <EGEVector.h>
#include <EGEScene.h>
#include <EGERandom.h>
#include <EGEParticle.h>

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

    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /* Initializes emitter from dictionary. */
    virtual bool initialize(const Dictionary& params);
    /*  Starts system. 
     *  @note This resets all data. 
     */
    void start();
    /* Updates object. */
    void update(const Time& time);

    /* Sets system life span. Negative time causes emitter to live infinitely. */
    void setLifeSpan(const Time& time);
    /* Sets maximum number of particles. */
    void setParticleMaxCount(s32 count);
    /* Sets emission rate. */
    void setEmissionRate(s32 rate);
    /*! Returns number of active particles. */
    inline s32 activeParticlesCount() const { return m_activeParticlesCount; }

    /* Sets particle start size. */
    void setParticleStartSize(const Vector2f& size);
    /* Sets particle start size variance. */
    void setParticleStartSizeVariance(const Vector2f& variance);
    /* Sets particle end size. */
    void setParticleEndSize(const Vector2f& size);
    /* Sets particle end size variance. */
    void setParticleStartEndVariance(const Vector2f& variance);
    /* Sets particle life span. */
    void setParticleLifeSpan(const Time& duration);
    /* Sets particle life span variance. */
    void setParticleLifeSpanVariance(const Time& variance);
    /* Sets particle start color. */
    void setParticleStartColor(const Color& color);
    /* Sets particle start color variance. */
    void setParticleStartColorVariance(const Color& variance);
    /* Sets particle end color. */
    void setParticleEndColor(const Color& color);
    /* Sets particle end color variance. */
    void setParticleEndColorVariance(const Color& variance);
   
    /* Sets material. */
    void setMaterial(const PMaterial& material);

    /* Adds affector. */
    void addAffector(PParticleAffector& affector);
    /* Removes given affector. */
    void removeAffector(PParticleAffector& affector);

    /*! Returns render component. */
    inline const PRenderComponent& renderComponent() const { return m_renderData; }

  private:

    /*! Returns TRUE if there is no available space for new particle. */
    inline bool isFull() const { return m_activeParticlesCount == m_particleMaxCount; }
    /* Allocates particles data. */
    bool allocateParticlesData();
    /* SceneNodeObject override. Adds object render data for rendering with given renderer. */
    bool addForRendering(Renderer* renderer) override;
    /* Applies affectors. */
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
    /*! Render data. */
    PRenderComponent m_renderData;
    /*! Array of particles. */
    ParticleDataArray m_particles;
    /*! Random generator. */
    Random m_random;
    /*! List of affectors. */
    ParticleAffectorList m_affectors;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_PARTICLEEMITTER_H
