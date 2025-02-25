#include "Core/Graphics/Particle/ParticleEmitter.h"
#include "EGEApplication.h"
#include "EGEGraphics.h"
#include "EGERenderSystem.h"
#include "EGERenderer.h"
#include "EGERenderComponent.h"
#include "EGEStringUtils.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

using NVertexBuffer::VertexElementSemanticArray;

  // TAGE
  static bool pointSprite = false;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ParticleEmitter)
EGE_DEFINE_DELETE_OPERATORS(ParticleEmitter)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ParticleEmitter::ParticleEmitter(Application* app, const String& name) : SceneNodeObject(name),
                                                                         m_active(false), 
                                                                         m_lifeDuration(0.0f), 
                                                                         m_activeParticlesCount(0), 
                                                                         m_emitCount(0.0f)
{
  // initialize to default values
  Dictionary params;
  initialize(params);

  VertexDeclaration declaration;

  if (pointSprite)
  {
    // Point sprites have:
    // - position
    // - color
    // - size (optionally, 1 float)
    if ( ! declaration.addElement(NVertexBuffer::VES_POSITION_XYZ) ||
         ! declaration.addElement(NVertexBuffer::VES_COLOR_RGBA) || 
         ! declaration.addElement(NVertexBuffer::VES_POINT_SPRITE_SIZE))
    {
      // error!
      declaration.clear();
    }
  }
  else
  {
    if ( ! declaration.addElement(NVertexBuffer::VES_POSITION_XYZ) ||
         ! declaration.addElement(NVertexBuffer::VES_TEXTURE_UV) ||
         ! declaration.addElement(NVertexBuffer::VES_COLOR_RGBA))
    {
      // error!
      declaration.clear();
    }
  }

  // create render data
  if (0 != declaration.vertexSize())
  {
    m_renderData = ege_new RenderComponent(app, name, declaration, EGEGraphics::RP_MAIN, pointSprite ? EGEGraphics::RPT_POINTS : EGEGraphics::RPT_TRIANGLES,
                                           NVertexBuffer::UT_DYNAMIC_WRITE_DONT_CARE);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ParticleEmitter::~ParticleEmitter()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ParticleEmitter::initialize(const Dictionary& params)
{
  bool error = false;

  // decompose params
  m_lifeSpan                    = params.value("life-span", "-1.0").toFloat(&error);
  m_particleMaxCount            = params.value("max-particle-count", "100").toInt(&error);
  m_emissionRate                = params.value("emission-rate", "15").toInt(&error);
  m_localSpace                  = params.value("local-space", "false").toBool(&error);
  m_particleStartSize           = StringUtils::ToVector2f(params.value("particle-start-size", "10 10"), &error);
  m_particleStartSizeVariance   = StringUtils::ToVector2f(params.value("particle-start-size-variance", "0 0"), &error);
  m_particleEndSize             = StringUtils::ToVector2f(params.value("particle-end-size", "0 0"), &error);
  m_particleEndSizeVariance     = StringUtils::ToVector2f(params.value("particle-end-size-variance", "0 0"), &error);
  m_particleLifeSpan            = params.value("particle-life-span", "5").toFloat(&error);
  m_particleLifeSpanVariance    = params.value("particle-life-span-variance", "0").toFloat(&error);
  m_particleStartColor          = StringUtils::ToColor(params.value("particle-start-color", "0 0 0 1"), &error);
  m_particleStartColorVariance  = StringUtils::ToColor(params.value("particle-start-color-variance", "0 0 0 0"), &error);
  m_particleEndColor            = StringUtils::ToColor(params.value("particle-end-color", "1 1 1 1"), &error);
  m_particleEndColorVariance    = StringUtils::ToColor(params.value("particle-end-color-variance", "0 0 0 0"), &error);
  m_particleSpinSpeed           = params.value("particle-spin-speed", "0").toFloat(&error);
  m_particleSpinSpeedVariance   = params.value("particle-spin-speed-variance", "0").toFloat(&error);

  return !error;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
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
void ParticleEmitter::stop()
{
  m_active = false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ParticleEmitter::update(const Time& time)
{
  // store time passed in seconds for optimization purposes
  float32 timeInSeconds = time.seconds();

  // process new particles only when active
  if (m_active)
  {
    // check if finite life span is set
    if (0 <= m_lifeSpan.microseconds())
    {
      // update life time
      m_lifeDuration += time;
      if (m_lifeDuration >= m_lifeSpan)
      {
        // life came to an end
        m_active = false;
        return;
      }
    }

    // update particles to emit counter
    m_emitCount += m_emissionRate * timeInSeconds;

    //EGE_PRINT("To emit: %f cap: %d active: %d", m_emitCount, m_particleMaxCount, m_activeParticlesCount);

    // add as much as we can
    while (!isFull() && (1.0f <= m_emitCount))
    {
      // add new particle and increase number of active particles
      initializeParticle(m_activeParticlesCount++);

      // emit
      emit particleSpawned(m_particles[m_activeParticlesCount - 1]);

      // decrement particle count for emission
      m_emitCount -= 1.0f;
    }
  }

  s32 i = 0;
  while (i < m_activeParticlesCount)
  {
    EGEParticle::ParticleData& particleData = m_particles[i];

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

      // update velocity
      particleData.velocity += particleData.acceleration * timeInSeconds;

      // update position
      particleData.position += particleData.velocity * timeInSeconds;

      // update spin
      particleData.spin += particleData.spinDelta.radians() * timeInSeconds;

      // go to next particle
      ++i;
    }
    else
    {
      // emit
      emit particleDied(particleData, i);

      // check if this is not last active particle
			if (i != m_activeParticlesCount - 1)
      {
        // move last active particle onto this one
				m_particles[i] = m_particles[m_activeParticlesCount - 1];
      }

      // decrease number of active particles
			--m_activeParticlesCount;
    }
  }

  // apply affectors
  applyAffectors(time);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ParticleEmitter::setLifeSpan(const Time& time)
{
  m_lifeSpan = time;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ParticleEmitter::setEmissionRate(s32 rate)
{
  EGE_ASSERT(0 <= rate);
  m_emissionRate = rate;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ParticleEmitter::setParticleMaxCount(s32 count)
{
  EGE_ASSERT(0 <= count);

  // store new size
  m_particleMaxCount = count;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ParticleEmitter::setParticleStartSize(const Vector2f& size)
{
  m_particleStartSize = size; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ParticleEmitter::setParticleStartSizeVariance(const Vector2f& variance)
{
  m_particleStartSizeVariance = variance; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ParticleEmitter::setParticleEndSize(const Vector2f& size)
{
  m_particleEndSize = size;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ParticleEmitter::setParticleStartEndVariance(const Vector2f& variance)
{
  m_particleEndSizeVariance = variance;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ParticleEmitter::setParticleLifeSpan(const Time& duration)
{
  m_particleLifeSpan = duration;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ParticleEmitter::setParticleLifeSpanVariance(const Time& variance)
{
  m_particleLifeSpanVariance = variance;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ParticleEmitter::setParticleStartColor(const Color& color)
{
  m_particleStartColor = color;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ParticleEmitter::setParticleStartColorVariance(const Color& variance)
{
  m_particleStartColorVariance = variance;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ParticleEmitter::setParticleEndColor(const Color& color)
{
  m_particleEndColor = color;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ParticleEmitter::setParticleEndColorVariance(const Color& variance)
{
  m_particleEndColorVariance = variance;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ParticleEmitter::setParticleSpinSpeed(float32 speed)
{
  m_particleSpinSpeed = speed;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ParticleEmitter::setParticleSpinSpeedVariance(float32 variance)
{
  m_particleSpinSpeedVariance = variance;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ParticleEmitter::allocateParticlesData()
{
  // resize array
  m_particles.resize(m_particleMaxCount);

  // properly adjust active particles count
  m_activeParticlesCount = Math::Min(m_activeParticlesCount, m_particleMaxCount);

  // update render data
  if (!m_renderData->vertexBuffer()->setSize(m_particleMaxCount * ((pointSprite) ? 1 : 6)))
  {
    // error!
    return false;
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ParticleEmitter::addForRendering(IRenderer* renderer, const Matrix4f& transform)
{
  const u32 vertexCount = m_activeParticlesCount * ((pointSprite) ? 1 : 6);

  // update vertex data
  if (m_renderData->vertexBuffer()->setSize(vertexCount))
  {
    float32* data = reinterpret_cast<float32*>(m_renderData->vertexBuffer()->lock(0, vertexCount));
    if (data)
    {
      // go thru all active particles
      if (pointSprite)
      {
        // calculate point size
        // NOTE: It is not always possible to define size per particle. That is why, we calculate the size here for entire batch. Additionally, we store size
        //       per sprite (based on current data) so renderer with proper capabilities can utilize it. Anyways, point sprites can only be square'ish.
        float32 size = Math::Max(m_particleStartSize.x + m_particleStartSizeVariance.x, m_particleStartSize.y + m_particleStartSizeVariance.y);
        size = Math::Max(size, m_particleEndSize.x + m_particleEndSizeVariance.x);
        size = Math::Max(size, m_particleEndSize.y + m_particleEndSizeVariance.y);

        // set point size
        m_renderData->setPointSize(size);

        for (s32 i = 0; i < m_activeParticlesCount; ++i)
        {
          const EGEParticle::ParticleData& particleData = m_particles[i];

          // NOTE: point sprites position determines sprite center point
          *data++ = particleData.position.x;
          *data++ = particleData.position.y;
          *data++ = particleData.position.z;
          *data++ = particleData.color.red;
          *data++ = particleData.color.green;
          *data++ = particleData.color.blue;
          *data++ = particleData.color.alpha;
          *data++ = Math::Max(particleData.size.x, particleData.size.y);

          // NOTE: Point sprites auto texture coords generator, generates them in non-OpenGL mode, ie like DX - 0 is at top 1 is at bottom
        }
      }
      else
      {
        const Matrix4f& viewMatrix = m_renderData->app()->graphics()->renderSystem()->viewMatrix();

        Vector3f right(viewMatrix.data[0], viewMatrix.data[1], viewMatrix.data[2]);
        Vector3f up(viewMatrix.data[4], viewMatrix.data[5], viewMatrix.data[6]);
        Vector3f look(viewMatrix.data[8], viewMatrix.data[9], viewMatrix.data[10]);

        for (s32 i = 0; i < m_activeParticlesCount; ++i)
        {
          const EGEParticle::ParticleData& particleData = m_particles[i];

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

          Vector3f halfSize(particleData.size.x * 0.5f, particleData.size.y * 0.5f, 0);
          Vector3f point0;
          Vector3f point1;
          Vector3f point2;
          Vector3f point5;

          // calculate positions
          // NOTE: we destinguish two cases here: with and without rotation
          if (0.0f == particleData.spinDelta.radians())
          {
            point0 = particleData.position + ((-right - up) * halfSize);
            point1 = particleData.position + ((-right + up) * halfSize);
            point2 = particleData.position + (( right + up) * halfSize);
            point5 = particleData.position + (( right - up) * halfSize);
          }
          else
          {
            const Quaternionf quat = Math::CreateQuaternion(look, particleData.spin);
          
            // take rotation into account
            point0 = particleData.position + quat * ((-right - up) * halfSize);
            point1 = particleData.position + quat * ((-right + up) * halfSize);
            point2 = particleData.position + quat * (( right + up) * halfSize);
            point5 = particleData.position + quat * (( right - up) * halfSize);
          }

          *data++ = point0.x;
          *data++ = point0.y;
          *data++ = point0.z;
          *data++ = 0;
          *data++ = 1;
          *data++ = particleData.color.red;
          *data++ = particleData.color.green;
          *data++ = particleData.color.blue;
          *data++ = particleData.color.alpha;

          *data++ = point1.x;
          *data++ = point1.y;
          *data++ = point1.z;
          *data++ = 0;
          *data++ = 0;
          *data++ = particleData.color.red;
          *data++ = particleData.color.green;
          *data++ = particleData.color.blue;
          *data++ = particleData.color.alpha;

          *data++ = point2.x;
          *data++ = point2.y;
          *data++ = point2.z;
          *data++ = 1;
          *data++ = 0;
          *data++ = particleData.color.red;
          *data++ = particleData.color.green;
          *data++ = particleData.color.blue;
          *data++ = particleData.color.alpha;

          *data++ = point0.x;
          *data++ = point0.y;
          *data++ = point0.z;
          *data++ = 0;
          *data++ = 1;
          *data++ = particleData.color.red;
          *data++ = particleData.color.green;
          *data++ = particleData.color.blue;
          *data++ = particleData.color.alpha;

          *data++ = point2.x;
          *data++ = point2.y;
          *data++ = point2.z;
          *data++ = 1;
          *data++ = 0;
          *data++ = particleData.color.red;
          *data++ = particleData.color.green;
          *data++ = particleData.color.blue;
          *data++ = particleData.color.alpha;

          *data++ = point5.x;
          *data++ = point5.y;
          *data++ = point5.z;
          *data++ = 1;
          *data++ = 1;
          *data++ = particleData.color.red;
          *data++ = particleData.color.green;
          *data++ = particleData.color.blue;
          *data++ = particleData.color.alpha;
        }
      }
    }

    m_renderData->vertexBuffer()->unlock((data) ? (data - 1) : NULL);
  }

  if (0 < m_activeParticlesCount)
  {
    renderer->addForRendering(m_renderData, transform);
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ParticleEmitter::setMaterial(const PMaterial& material)
{
  m_renderData->setMaterial(material);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ParticleEmitter::applyAffectors(const Time& time)
{
  // go thru all affectors
  for (ParticleAffectorList::iterator it = m_affectors.begin(); it != m_affectors.end(); ++it)
  {
    PParticleAffector& affector = *it;

    affector->apply(time, m_particles, m_activeParticlesCount);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ParticleEmitter::addAffector(PParticleAffector& affector)
{
  affector->m_emitter = this;
  m_affectors.push_back(affector);

  // check if affector needs detailed particle info
  if (affector->detailedParticleInfoRequired())
  {
    ege_connect(this, particleSpawned, affector.object(), ParticleAffector::onNewParticleSpawned);
    ege_connect(this, particleDied, affector.object(), ParticleAffector::onParticleDied);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ParticleEmitter::removeAffector(PParticleAffector& affector)
{
  if (m_affectors.contains(affector))
  {
    affector->m_emitter = NULL;
    m_affectors.remove(affector);

    // check if affector needed detailed particle info
    if (affector->detailedParticleInfoRequired())
    {
      ege_disconnect(this, particleSpawned, affector.object(), ParticleAffector::onNewParticleSpawned);
      ege_disconnect(this, particleDied, affector.object(), ParticleAffector::onParticleDied);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END