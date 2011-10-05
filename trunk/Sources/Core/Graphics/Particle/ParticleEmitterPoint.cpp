#include "Core/Graphics/Particle/ParticleEmitterPoint.h"
#include <EGEGraphics.h>
#include <EGEDebug.h>

EGE_NAMESPACE

  // TAGE
  static bool pointSprite = true;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(ParticleEmitterPoint)
EGE_DEFINE_DELETE_OPERATORS(ParticleEmitterPoint)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ParticleEmitterPoint::ParticleEmitterPoint(Application* app, const String& name) : ParticleEmitter(app, name)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ParticleEmitterPoint::~ParticleEmitterPoint()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance of emitter. This method is a registration method for factory. */
PParticleEmitter ParticleEmitterPoint::Create(Application* app, const String& name)
{
  return ege_new ParticleEmitterPoint(app, name);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! ParticleEmitter override. Initializes emitter from dictionary. */
bool ParticleEmitterPoint::initialize(const Dictionary& params)
{
  // initialize base
  bool error = !ParticleEmitter::initialize(params);

  // decompose params
  m_emissionAngle                 = Angle::FromDegrees(params.value("emission-angle", "360").toFloat(&error));
  m_emissionAngleVariance         = Angle::FromDegrees(params.value("emission-angle-variance", "0").toFloat(&error));
  m_emissionDirection             = params.value("emission-direction", "1 0 0").toVector3f(&error);
  m_emissionDirectionMask         = params.value("emission-direction-mask", "1 1 1").toVector3f(&error);
  m_particleStartPositionVariance = params.value("particle-start-position-variance", "0 0 0").toVector3f(&error);
  m_particleSpeed                 = params.value("particle-speed", "1.0").toFloat(&error);
  m_particleSpeedVariance         = params.value("particle-speed-variance", "0").toFloat(&error);

  return !error;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets emission angle. */
void ParticleEmitterPoint::setEmissionAngle(const Angle& angle)
{
  m_emissionAngle = angle;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets emission angle variance. */
void ParticleEmitterPoint::setEmissionAngleVariance(const Angle& variance)
{
  m_emissionAngleVariance = variance;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets particle speed. */
void ParticleEmitterPoint::setParticleSpeed(float32 speed)
{
  m_particleSpeed = speed;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets particle speed variance. */
void ParticleEmitterPoint::setParticleSpeedVariance(float32 variance)
{
  m_particleSpeedVariance = variance;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets emitter direction. */
void ParticleEmitterPoint::setEmissionDirection(const Vector3f& direction)
{
  m_emissionDirection = direction;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets emitter direction mask. */
void ParticleEmitterPoint::setEmissionDirectionMask(const Vector3f& directionMask)
{
  m_emissionDirectionMask = directionMask;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Initializes particle at given index. */
void ParticleEmitterPoint::initializeParticle(s32 index)
{
  EGE_ASSERT((0 <= index) && (index < static_cast<s32>(m_particles.size())));

  ParticleData& particleData = m_particles[index];

  float32 inverseLifeTimeSeconds = 1.0f;

  // calculate start position
	particleData.position = parentNode()->physics()->position().xyz() + m_particleStartPositionVariance * m_random(-1.0f, 1.0f);

	// calculate direction
	Angle angle = m_emissionAngle + m_emissionAngleVariance.radians() * m_random(-1.0f, 1.0f);
  angle *= 0.5f;
  particleData.direction = Math::RandomDeviant(&angle, &m_emissionDirection);
  particleData.direction.x *= m_emissionDirectionMask.x;
  particleData.direction.y *= m_emissionDirectionMask.y;
  particleData.direction.z *= m_emissionDirectionMask.z;
  particleData.direction.normalize();

  // apply speed
  particleData.direction *= m_particleSpeed + m_particleSpeedVariance * m_random(-1.0f, 1.0f);

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
