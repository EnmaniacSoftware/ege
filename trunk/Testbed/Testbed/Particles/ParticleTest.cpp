#include "App.h"
#include "ParticleTest.h"
#include <EGESignal.h>
#include <EGEResources.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#define ORTHO

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ParticleTest::ParticleTest(App* app) : Test(app)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ParticleTest::~ParticleTest()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Returns test name. */
String ParticleTest::name() const
{
  return "Particle";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Initializes test. */
bool ParticleTest::initialize()
{
  // get window render target (created thru config options)
  PRenderWindow window = app()->graphics()->renderTarget(EGE_PRIMARY_RENDER_TARGET_NAME);

  // create new camera
  PCamera camera = ege_new Camera("myCamera", app()->sceneManager());

#ifdef ORTHO
  // setup orthographic projection
  camera->setProjectionType(EGE::Frustum::ORTHOGRAPHIC);
  camera->setOrthoSettings(Rectf(0, 0, window->width() * 1.0f, window->height() * 1.0f));
  camera->setNearPlaneDistance(-1);
  camera->setFarPlaneDistance(1);
#endif

  camera->setLookAt(Vector3f(0, 0, 0));
  camera->physics()->setPosition(Vector4f(0, 0, -50));

  // create new viewport for render window
  PViewport viewport = window->addViewport("mainview", camera);
  if (NULL == viewport)
  {
    // error!
    return false;
  }

  viewport->setClearBufferTypes(Viewport::BUFFER_TYPE_COLOR);
  viewport->setClearColor(Color::BLUE);

  // load resources
  if (EGE_SUCCESS != app()->resourceManager()->loadGroup("particle-test"))
  {
    // error!
    return false;
  }

  // get particle material resource
  PResourceMaterial resource = app()->resourceManager()->resource(RESOURCE_NAME_MATERIAL, "particle");

  // create particle from resource
  PResourceParticleEmitter emitterResource = app()->resourceManager()->resource(RESOURCE_NAME_PARTICLE_EMITTER, "point-emitter-example");
  m_emitter = emitterResource->createInstance();
  m_emitter->setMaterial(resource->createInstance());
  m_emitter->start();

  // create particle emitter from scratch
  m_emitter2 = ege_new ParticleEmitterPoint(app(), "ala");
  m_emitter2->setParticleMaxCount(1500);
  m_emitter2->setParticleSpeed(50.0f);
  m_emitter2->setParticleSpeedVariance(10.0f);
  m_emitter2->setEmissionDirection(Vector3f(1.0f, 0.0f, 0.0f));
  m_emitter2->setEmissionDirectionMask(Vector3f(1.0f, 1.0f, 0.0f));
  m_emitter2->setEmissionAngle(Angle::FromDegrees(45.0f));
  m_emitter2->setParticleStartSize(Vector2f(10, 10));
  m_emitter2->setParticleEndSize(Vector2f(10, 10));
  m_emitter2->setEmissionRate(250);
  m_emitter2->setMaterial(resource->createInstance());
  m_emitter2->start();

  app()->sceneManager()->rootNode()->createChildSceneNode("emitter-1")->attachObject(m_emitter);
  app()->sceneManager()->rootNode()->childNode("emitter-1")->physics()->setPosition(Vector4f(200, 200, 0));
  app()->sceneManager()->rootNode()->attachObject(m_emitter2);
  app()->sceneManager()->rootNode()->physics()->setPosition(Vector4f(200, 400, 0));

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Updates test. */
void ParticleTest::update(const Time& time)
{
  if (m_emitter)
  {
    m_emitter->update(time);
    EGE_PRINT("%d", m_emitter->activeParticlesCount());
  }

  if (m_emitter2)
  {
    m_emitter2->update(time);
    EGE_PRINT("%d", m_emitter2->activeParticlesCount());
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
