#ifndef EGE_CORE_PARTICLESYSTEM_H
#define EGE_CORE_PARTICLESYSTEM_H

/** 
*   Particle system is an object responsible for creation and managing the particles according to predefined data.
*/

#include <EGE.h>
#include <EGEVertexBuffer.h>
#include <EGEDynamicArray.h>
#include <EGETime.h>
#include <EGEColor.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DECLARE_SMART_CLASS(ParticleSystem, PParticleSystem)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class ParticleSystem : public Object
{
  public:

    ParticleSystem(Application* app);
   ~ParticleSystem();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /*  Starts system. 
     *  @note This resets all data. 
     */
    void start();
    /* Updates object. */
    void update(const Time& time);
    /* Sets system life span. */
    void setLifeSpan(const Time& time);
    /* Sets maximum number of particles. */
    void setParticleMaxCount(s32 count);
    /* Sets emission rate. */
    void setEmissionRate(s32 rate);
    /* Sets particle start size. */
    void setParticleStartSize(float32 size);
    /* Sets particle start size variance. */
    void setParticleStartSizeVariance(float32 variance);
    /* Sets particle end size. */
    void setParticleEndSize(float32 size);
    /* Sets particle end size variance. */
    void setParticleStartEndVariance(float32 variance);
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

  private:

    /*! Returns TRUE if there is no available space for new particle. */
    inline bool isFull() const { return m_activeParticlesCount == m_particleMaxCount; }
    /* Initializes particle at given index. */
    void initializeParticle(s32 index);
    /* Deallocates particles data array. */
    void deallocatesParticleDataArray();

  private:

    /*! Particle data structure. */
    struct ParticleData
    {
      Vector3f position;
    };

  private:

    typedef DynamicArray<ParticleData> ParticleDataArray;

  private:

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
    float32 m_particleStartSize;
    /*! Particles start size variance. */
    float32 m_particleStartSizeVariance;
    /*! Particles end size. */
    float32 m_particleEndSize;
    /*! Particles end size variance. */
    float32 m_particleEndSizeVariance;
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
    /*! Array of particles. */
    ParticleDataArray m_particles;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_PARTICLESYSTEM_H
