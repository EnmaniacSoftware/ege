#include "App.h"
#include "ThreadsTest.h"
#include <EGEResources.h>
#include <EGEOverlay.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#define ORTHO

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ThreadsTest::ThreadsTest(App* app) : Test(app)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ThreadsTest::~ThreadsTest()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Returns test name. */
String ThreadsTest::name() const
{
  return "Threads";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Initializes test. */
bool ThreadsTest::initialize()
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

  PResourceFont fontResource = app()->resourceManager()->resource(RESOURCE_NAME_FONT, "debug-font");
  if (fontResource)
  {
    for (s32 i = 0; i < THREADS_COUNT; ++i)
    {
      PTextOverlay overlay = ege_new TextOverlay(app(), String::Format("thread-%d-counter", i));
      overlay->setFont(fontResource->font());
      app()->overlayManager()->add(overlay);
      overlay->physics()->setPosition(Vector4f(0, 80 + i * 40.0f, 0));
    }
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Updates test. */
void ThreadsTest::update(const Time& time)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Pointer event receiver. */
void ThreadsTest::pointerEvent(PPointerData data)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
