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

  // create particle emitter
  m_emitter = ege_new ParticleEmitter(app(), "ParticleEmitter1");
  m_emitter->setParticleMaxCount(1500);
  m_emitter->setParticleSpeed(50.0f);
  m_emitter->setParticleSpeedVariance(10.0f);
  m_emitter->setEmissionDirection(Vector3f(1.0f, 0.0f, 0.0f));
  m_emitter->setEmissionDirectionMask(Vector3f(1.0f, 1.0f, 0.0f));
  m_emitter->setEmissionAngle(Angle::FromDegrees(10.0f));
  m_emitter->setParticleStartSize(Vector2f(10, 10));
  m_emitter->setParticleEndSize(Vector2f(10, 10));
  m_emitter->setEmissionRate(250);

  PResourceMaterial resource = app()->resourceManager()->resource(RESOURCE_NAME_MATERIAL, "particle");
  m_emitter->setMaterial(resource->createInstance());

  app()->sceneManager()->rootNode()->attachObject(m_emitter);
  app()->sceneManager()->rootNode()->physics()->setPosition(Vector4f(200, 200, 0));
  m_emitter->start();

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Updates test. */
void ParticleTest::update(const Time& time)
{
  m_emitter->update(time);

  EGE_PRINT("%d", m_emitter->activeParticlesCount());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
