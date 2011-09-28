#include "Core/Graphics/Particle/ParticleEmitter.h"
#include <EGEDebug.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(ParticleEmitter)
EGE_DEFINE_DELETE_OPERATORS(ParticleEmitter)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ParticleEmitter::ParticleEmitter(Application* app) : Object(app), m_active(false), m_lifeSpan(10.0f), m_lifeDuration(0.0f), m_particleMaxCount(100),
                                                     m_activeParticlesCount(0), m_emissionRate(15), m_particleStartSize(10), m_particleStartSizeVariance(0), 
                                                     m_particleEndSize(0), m_particleEndSizeVariance(0), m_particleLifeSpan(5.0f), 
                                                     m_particleLifeSpanVariance(0.0f), m_particleStartColor(Color::BLACK), 
                                                     m_particleStartColorVariance(Color::NONE), m_particleEndColor(Color::WHITE), 
                                                     m_particleEndColorVariance(Color::NONE)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ParticleEmitter::~ParticleEmitter()
{
  deallocatesParticleDataArray();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool ParticleEmitter::isValid() const
{
  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Starts system. 
 *  @note This resets all data. 
 */
void ParticleEmitter::start()
{
  // reset data
  m_lifeDuration = 0.0f;
  m_activeParticlesCount = 0;


  // make active
  m_active = true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates object. */
void ParticleEmitter::update(const Time& time)
{
  // process new particles only when active
  if (m_active)
  {
    // update life time
    m_lifeDuration += time;
    if (m_lifeDuration >= m_lifeSpan)
    {
      // life came to an end
      m_active = false;
      return;
    }

    // update particles to emit counter
    m_emitCount += m_emissionRate * time.seconds();

    // add as much as we can
    while (!isFull() && (1.0f <= m_emitCount))
    {
      // add new particle and increase number of active particles
      initializeParticle(m_activeParticlesCount++);

      // decrement particle count for emission
      m_emitCount -= 1.0f;
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets system life span. */
void ParticleEmitter::setLifeSpan(const Time& time)
{
  m_lifeSpan = time;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets emission rate. */
void ParticleEmitter::setEmissionRate(s32 rate)
{
  EGE_ASSERT(0 <= rate);
  m_emissionRate = rate;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets maximum number of particles. */
void ParticleEmitter::setParticleMaxCount(s32 count)
{
  EGE_ASSERT(0 <= count);
  m_particleMaxCount = count;

  // create new array
  ParticleDataArray newArray;
  newArray.reserve(count);

  // create particle data structs
  //for (s32 i = 0; i < count; ++i)
  //{
  //  // allocate particle data
  //  ParticleData* particle = (ParticleData*) EGE_MALLOC(sizeof(ParticleData));
  //  if (NULL == particle)
  //  {
  //    // error!
  //    for (s32 j = 0; j < i - 1; ++j)
  //    {
  //      EGE_FREE(newArray[i]);
  //    }

  //    return;
  //  }

  //  // check if there is old particle which should be copied over
  //  if (i < static_cast<s32>(m_particles.size()))
  //  {
  //    // copy old particle data
  //    EGE_MEMCPY(particle, &m_particles[i], sizeof(ParticleData));
  //  }

  //  // add to pool
  //  newArray.push_back(particle);
  //}

  //// deallocate current particle data array
  //deallocatesParticleDataArray();

  //// store new array
  //m_particles.copy(newArray);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets particle start size. */
void ParticleEmitter::setParticleStartSize(float32 size)
{
  m_particleStartSize = size; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets particle start size variance. */
void ParticleEmitter::setParticleStartSizeVariance(float32 variance)
{
  m_particleStartSizeVariance = variance; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets particle end size. */
void ParticleEmitter::setParticleEndSize(float32 size)
{
  m_particleEndSize = size;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets particle end size variance. */
void ParticleEmitter::setParticleStartEndVariance(float32 variance)
{
  m_particleEndSizeVariance = variance;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets particle life span. */
void ParticleEmitter::setParticleLifeSpan(const Time& duration)
{
  m_particleLifeSpan = duration;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets particle life span variance. */
void ParticleEmitter::setParticleLifeSpanVariance(const Time& variance)
{
  m_particleLifeSpanVariance = variance;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets particle start color. */
void ParticleEmitter::setParticleStartColor(const Color& color)
{
  m_particleStartColor = color;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets particle start color variance. */
void ParticleEmitter::setParticleStartColorVariance(const Color& variance)
{
  m_particleStartColorVariance = variance;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets particle end color. */
void ParticleEmitter::setParticleEndColor(const Color& color)
{
  m_particleEndColor = color;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets particle end color variance. */
void ParticleEmitter::setParticleEndColorVariance(const Color& variance)
{
  m_particleEndColorVariance = variance;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Initializes particle at given index. */
void ParticleEmitter::initializeParticle(s32 index)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Deallocates particles data array. */
void ParticleEmitter::deallocatesParticleDataArray()
{
  // deallocate old pool
  for (ParticleDataArray::iterator it = m_particles.begin(); it != m_particles.end(); ++it)
  {
//    EGE_FREE(*it);
  }
  m_particles.clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
