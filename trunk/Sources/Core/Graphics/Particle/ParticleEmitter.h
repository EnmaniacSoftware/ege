#ifndef EGE_CORE_PARTICLEEMITTER_H
#define EGE_CORE_PARTICLEEMITTER_H

/** 
*   Particle emitter is an object responsible for creation and managing the particles according to predefined data.
*/

#include <EGE.h>
#include <EGEVertexBuffer.h>
#include <EGEDynamicArray.h>
#include <EGETime.h>
#include <EGEColor.h>
#include <EGEVector.h>
#include <EGEScene.h>
#include <EGERandom.h>
#include <EGEParticle.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DECLARE_SMART_CLASS(ParticleEmitter, PParticleEmitter)

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
    /*  Starts system. 
     *  @note This resets all data. 
     */
    void start();
    /* Updates object. */
    void update(const Time& time);
    /* Sets mode. */
    void setMode(EGEParticle::EmitterMode mode);
    /* Sets system life span. */
    void setLifeSpan(const Time& time);
    /* Sets maximum number of particles. */
    void setParticleMaxCount(s32 count);
    /* Sets emission rate. */
    void setEmissionRate(s32 rate);
    /* Sets emission angle. */
    void setEmissionAngle(const Angle& angle);
    /* Sets emission angle variance. */
    void setEmissionAngleVariance(const Angle& variance);
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

  private:

    /*! Returns TRUE if there is no available space for new particle. */
    inline bool isFull() const { return m_activeParticlesCount == m_particleMaxCount; }
    /* Initializes particle at given index. */
    void initializeParticle(s32 index);

  private:

    /*! Particle data structure. */
    struct ParticleData
    {
      Vector3f position;            /*!< Current position. */ 

      Color color;                  /*!< Current color. */
      Color colorDelta;             /*!< Color delta. */

      Vector2f size;                /*!< Current size. */
      Vector2f sizeDelta;           /*!< Size delta. */

      Time timeLeft;                /*!< Time left to die. */

	    union 
      {
		    // Mode A: gravity, direction, radial accel, tangential accel
		    struct 
        {
			    float32 dirX;
			    float32 dirY;
			    float32 dirZ;
			    float32	radialAccel;
			    float32	tangentialAccel;
		    
        } GravityMode;
	
		    // Mode B: radius mode
		    struct 
        {
			    float32 angle;
			    float32	degreesPerSecond;
			    float32	adius;
			    float32	deltaRadius;

        } RadialMode;

	    } mode;
    };

  private:

    typedef DynamicArray<ParticleData> ParticleDataArray;

  private:

    /*! Emitter mode. */
    EGEParticle::EmitterMode m_mode;
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
    /*! Particle emission angle. */
    Angle m_emissionAngle;
    /*! Particle emission angle variance. */
    Angle m_emissionAngleVariance;
    /*! Particles start position variance. */
    Vector3f m_particleStartPositionVariance;
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
    /*! Array of particles. */
    ParticleDataArray m_particles;
    /*! Random generator. */
    Random m_random;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_PARTICLEEMITTER_H
