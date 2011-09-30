#include "Core/Graphics/Particle/ParticleEmitter.h"
#include <EGEDebug.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(ParticleEmitter)
EGE_DEFINE_DELETE_OPERATORS(ParticleEmitter)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ParticleEmitter::ParticleEmitter(Application* app, const String& name) : SceneNodeObject(name),
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
                                                                         m_particleEndColorVariance(Color::NONE),
                                                                         m_particleSpeed(1.0f),
                                                                         m_particleSpeedVariance(0.0f)
{
  // create render data
  m_renderData = ege_new RenderComponent(app, name, EGEGraphics::RP_MAIN);
  if (m_renderData)
  {
    m_renderData->vertexBuffer()->setSemantics(EGEVertexBuffer::ST_V3_T2_C4);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ParticleEmitter::~ParticleEmitter()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool ParticleEmitter::isValid() const
{
  return (NULL != m_renderData) && m_renderData->isValid();
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

  allocateParticlesData();

  // make active
  m_active = true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates object. */
void ParticleEmitter::update(const Time& time)
{
  // store time passed in seconds for optimization purposes
  float32 timeInSeconds = time.seconds();

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
    m_emitCount += m_emissionRate * timeInSeconds;

    // add as much as we can
    while (!isFull() && (1.0f <= m_emitCount))
    {
      // add new particle and increase number of active particles
      initializeParticle(m_activeParticlesCount++);

      // decrement particle count for emission
      m_emitCount -= 1.0f;
    }
  }

  float32* data = static_cast<float32*>(m_renderData->vertexBuffer()->lock(0, m_activeParticlesCount * 6));

  s32 i = 0;
  while (i < m_activeParticlesCount)
  {
    ParticleData& particleData = m_particles[i];

    // update time
    particleData.timeLeft -= time;

    // check if particle is still alive
    if (0 < particleData.timeLeft.microseconds())
    {
      // upate size
      particleData.size += particleData.sizeDelta * timeInSeconds;

      // update color
      particleData.color.red    += particleData.colorDelta.red * timeInSeconds;
      particleData.color.green  += particleData.colorDelta.green * timeInSeconds;
      particleData.color.blue   += particleData.colorDelta.blue * timeInSeconds;
      particleData.color.alpha  += particleData.colorDelta.alpha * timeInSeconds;

      // update position
      particleData.position += particleData.direction * timeInSeconds;

      // go to next particle
      ++i;
    }
    else
    {
      // check if this is not last active particle
			if (i != m_activeParticlesCount - 1)
      {
        // move last active particle onto this one
				m_particles[i] = m_particles[m_activeParticlesCount - 1];
      }

      // decrease number of active particles
			--m_activeParticlesCount;
			
			//if ( particleCount == 0 && autoRemoveOnFinish_ ) {
			//	[self unscheduleUpdate];
			//	[parent_ removeChild:self cleanup:YES];
			//	return;
			//}
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

  // store new size
  m_particleMaxCount = count;
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
/*! Sets particle speed. */
void ParticleEmitter::setParticleSpeed(float32 speed)
{
  m_particleSpeed = speed;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets particle speed variance. */
void ParticleEmitter::setParticleSpeedVariance(float32 variance)
{
  m_particleSpeedVariance = variance;
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
  particleData.direction = Math::RandomDeviant(&angle, &m_emissionDirection);
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
/*! Allocates particles data. */
bool ParticleEmitter::allocateParticlesData()
{
  // resize array
  m_particles.resize(m_particleMaxCount);

  // properly adjust active particles count
  m_activeParticlesCount = Math::Min(m_activeParticlesCount, m_particleMaxCount);

  // update render data
  if (!m_renderData->vertexBuffer()->create(m_particleMaxCount * 6))
  {
    // error!
    return false;
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! SceneNodeObject override. Adds object render data for rendering with given renderer. */
bool ParticleEmitter::addForRendering(Renderer* renderer)
{
      // Quad looks like follows:
      //
      //   (0,3)  (5)
      //    *------*
      //    |\     |
      //    | \Tri2|
      //    |  \   |
      //    |   \  |
      //    |    \ |
      //    |Tri1 \|
      //    |      |
      //    *------*
      //   (1)   (2,4)

      //*data++ = pos.x;
      //*data++ = pos.y;
      //*data++ = 0;
      //*data++ = glyphData->m_textureRect.x;
      //*data++ = glyphData->m_textureRect.y;
      //*data++ = color.red;
      //*data++ = color.green;
      //*data++ = color.blue;
      //*data++ = color.alpha;

      //*data++ = pos.x;
      //*data++ = pos.y + height;
      //*data++ = 0;
      //*data++ = glyphData->m_textureRect.x;
      //*data++ = glyphData->m_textureRect.y + glyphData->m_textureRect.height;
      //*data++ = color.red;
      //*data++ = color.green;
      //*data++ = color.blue;
      //*data++ = color.alpha;

      //*data++ = pos.x + width;
      //*data++ = pos.y + height;
      //*data++ = 0;
      //*data++ = glyphData->m_textureRect.x + glyphData->m_textureRect.width;
      //*data++ = glyphData->m_textureRect.y + glyphData->m_textureRect.height;
      //*data++ = color.red;
      //*data++ = color.green;
      //*data++ = color.blue;
      //*data++ = color.alpha;

      //*data++ = pos.x;
      //*data++ = pos.y;
      //*data++ = 0;
      //*data++ = glyphData->m_textureRect.x;
      //*data++ = glyphData->m_textureRect.y;
      //*data++ = color.red;
      //*data++ = color.green;
      //*data++ = color.blue;
      //*data++ = color.alpha;

      //*data++ = pos.x + width;
      //*data++ = pos.y + height;
      //*data++ = 0;
      //*data++ = glyphData->m_textureRect.x + glyphData->m_textureRect.width;
      //*data++ = glyphData->m_textureRect.y + glyphData->m_textureRect.height;
      //*data++ = color.red;
      //*data++ = color.green;
      //*data++ = color.blue;
      //*data++ = color.alpha;

      //*data++ = pos.x + width;
      //*data++ = pos.y;
      //*data++ = 0;
      //*data++ = glyphData->m_textureRect.x + glyphData->m_textureRect.width;
      //*data++ = glyphData->m_textureRect.y;
      //*data++ = color.red;
      //*data++ = color.green;
      //*data++ = color.blue;
      //*data++ = color.alpha;

  Matrix4f matrix = Matrix4f::IDENTITY;
  return renderer->addForRendering(matrix, m_renderData);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
