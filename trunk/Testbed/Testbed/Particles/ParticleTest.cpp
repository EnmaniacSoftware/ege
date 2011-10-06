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

  app()->sceneManager()->rootNode()->createChildSceneNode("emitter-1")->attachObject(m_emitter);
  app()->sceneManager()->rootNode()->childNode("emitter-1")->physics()->setPosition(Vector4f(200, 200, 0));

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
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
