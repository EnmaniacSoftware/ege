#include "Core/Graphics/Particle/ParticleEmitter.h"
#include <EGEDebug.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(ParticleEmitter)
EGE_DEFINE_DELETE_OPERATORS(ParticleEmitter)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ParticleEmitter::ParticleEmitter(Application* app, const String& name) : SceneNodeObject(name),
                                                                         m_mode(EGEParticle::EM_GRAVITY),
                                                                         m_active(false), 
                                                                         m_lifeSpan(10.0f), 
                                                                         m_lifeDuration(0.0f), 
                                                                         m_particleMaxCount(100), 
                                                                         m_activeParticlesCount(0), 
                                                                         m_emissionRate(15), 
                                                                         m_emitCount(0.0f),
                                                                         m_emissionAngle(EGEMath::TWO_PI),
                                                                         m_emissionAngleVariance(0),
                                                                         m_emissionDirection(Vector3f::UNIT_X),
                                                                         m_particleStartPositionVariance(Vector3f::ZERO), 
                                                                         m_particleStartSize(Vector2f(10, 10)), 
                                                                         m_particleStartSizeVariance(Vector2f(0, 0)), 
                                                                         m_particleEndSize(Vector2f(0, 0)), 
                                                                         m_particleEndSizeVariance(Vector2f(0, 0)), 
                                                                         m_particleLifeSpan(5.0f), 
                                                                         m_particleLifeSpanVariance(0.0f), 
                                                                         m_particleStartColor(Color::BLACK), 
                                                                         m_particleStartColorVariance(Color::NONE), 
                                                                         m_particleEndColor(Color::WHITE), 
                                                                         m_particleEndColorVariance(Color::NONE)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ParticleEmitter::~ParticleEmitter()
{
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
  m_emitCount = 0.0f;

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

  // resize array
  m_particles.resize(count);

  // store new size
  m_particleMaxCount = count;

  // properly adjust active particles count
  m_activeParticlesCount = Math::Min(m_activeParticlesCount, count);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets particle start size. */
void ParticleEmitter::setParticleStartSize(const Vector2f& size)
{
  m_particleStartSize = size; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets particle start size variance. */
void ParticleEmitter::setParticleStartSizeVariance(const Vector2f& variance)
{
  m_particleStartSizeVariance = variance; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets particle end size. */
void ParticleEmitter::setParticleEndSize(const Vector2f& size)
{
  m_particleEndSize = size;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets particle end size variance. */
void ParticleEmitter::setParticleStartEndVariance(const Vector2f& variance)
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
/*! Sets emission angle. */
void ParticleEmitter::setEmissionAngle(const Angle& angle)
{
  m_emissionAngle = angle;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets emission angle variance. */
void ParticleEmitter::setEmissionAngleVariance(const Angle& variance)
{
  m_emissionAngleVariance = variance;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets mode. */
void ParticleEmitter::setMode(EGEParticle::EmitterMode mode)
{
  m_mode = mode;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets emitter direction. */
void ParticleEmitter::setEmissionDirection(const Vector3f& direction)
{
  m_emissionDirection = direction;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Initializes particle at given index. */
void ParticleEmitter::initializeParticle(s32 index)
{
  EGE_ASSERT((0 <= index) && (index < static_cast<s32>(m_particles.size())));

  ParticleData& particleData = m_particles[index];

  float32 inverseLifeTimeSeconds = 1.0f;

  // calculate start position
	particleData.position = parentNode()->physics()->position().xyz() + m_particleStartPositionVariance * m_random(-1.0f, 1.0f);

	// calculate direction
	Angle angle = m_emissionAngle + m_emissionAngleVariance.radians() * m_random(-1.0f, 1.0f);
	
// the function Random() returns a float in the range [0,1]
  //float2 RandomDirection2D()
  //{  
  //  float azimuth = Random() * 2 * pi;  
  //  return float2(cos(azimuth), sin(azimuth);
  //}
  //
  //float3 RandomDirection3D()
  //{  
  //  float z = (2*Random()) - 1; // z is in the range [-1,1]  
  //  float2 planar = RandomDirection2D() * sqrt(1-z*z);  
  //  return float3(planar.x, planar.y, z);
  //}

  // initialize mode specific data
  if (EGEParticle::EM_GRAVITY == m_mode)
  {
    Vector3f dir = Math::RandomDeviant(&angle, &m_emissionDirection);
    dir.normalize();

    particleData.mode.GravityMode.dirX = dir.x;
    particleData.mode.GravityMode.dirY = dir.y;
    particleData.mode.GravityMode.dirZ = dir.z;
  }
  else if (EGEParticle::EM_RADIAL == m_mode)
  {
  }
  else
  {
      EGE_ASSERT(false && "Implement");
  }

	// Create a new Vector2f using the newAngle
//	Vector2f vector = Vector2fMake(cosf(newAngle), sinf(newAngle));
	
	// Calculate the vectorSpeed using the speed and speedVariance which has been passed in
//	float vectorSpeed = speed + speedVariance * RANDOM_MINUS_1_TO_1();
	
	// The particles direction vector is calculated by taking the vector calculated above and
	// multiplying that by the speed
	//particle->direction = Vector2fMultiply(vector, vectorSpeed);

	// calculate the particles life span using the life span and variance passed in
	particleData.timeLeft = m_particleLifeSpan + m_particleLifeSpanVariance * m_random(-1.0f, 1.0f);
	particleData.timeLeft.fromMicroseconds(Math::Max(0LL, particleData.timeLeft.microseconds()));
  if (0 < particleData.timeLeft.microseconds())
  {
    inverseLifeTimeSeconds = 1.0f / particleData.timeLeft.seconds();
  }

	// calculate particle start size
	particleData.size = m_particleStartSize + m_particleStartSizeVariance * m_random(-1.0f, 1.0f);

  // calulate particle end size
  Vector2f endSize = m_particleEndSize + m_particleEndSizeVariance * m_random(-1.0f, 1.0f);

	// calculate particle size change
	particleData.sizeDelta = (endSize - particleData.size) * inverseLifeTimeSeconds;

	// calculate start color
  particleData.color.red    = Math::Bound(m_particleStartColor.red + m_particleStartColorVariance.red * m_random(-1.0f, 1.0f), 0.0f, 1.0f);
  particleData.color.green  = Math::Bound(m_particleStartColor.green + m_particleStartColorVariance.green * m_random(-1.0f, 1.0f), 0.0f, 1.0f);
  particleData.color.blue   = Math::Bound(m_particleStartColor.blue + m_particleStartColorVariance.blue * m_random(-1.0f, 1.0f), 0.0f, 1.0f);
  particleData.color.alpha  = Math::Bound(m_particleStartColor.alpha + m_particleStartColorVariance.alpha * m_random(-1.0f, 1.0f), 0.0f, 1.0f);

  // calculate end color
  Color endColor;
  endColor.red    = Math::Bound(m_particleEndColor.red + m_particleEndColor.red * m_random(-1.0f, 1.0f), 0.0f, 1.0f);
  endColor.green  = Math::Bound(m_particleEndColor.green + m_particleEndColor.green * m_random(-1.0f, 1.0f), 0.0f, 1.0f);
  endColor.blue   = Math::Bound(m_particleEndColor.blue + m_particleEndColor.blue * m_random(-1.0f, 1.0f), 0.0f, 1.0f);
  endColor.alpha  = Math::Bound(m_particleEndColor.alpha + m_particleEndColor.alpha * m_random(-1.0f, 1.0f), 0.0f, 1.0f);

  // calculate color change
  particleData.colorDelta.red   = (endColor.red - particleData.color.red) * inverseLifeTimeSeconds;
  particleData.colorDelta.green = (endColor.green - particleData.color.green) * inverseLifeTimeSeconds;
  particleData.colorDelta.blue  = (endColor.blue - particleData.color.blue) * inverseLifeTimeSeconds;
  particleData.colorDelta.alpha = (endColor.alpha - particleData.color.alpha) * inverseLifeTimeSeconds;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
