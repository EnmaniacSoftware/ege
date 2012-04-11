#include "App.h"
#include "ImagedAnimationTest.h"
#include <EGESignal.h>
#include <EGEResources.h>
#include <EGEOverlay.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#define ORTHO

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ImagedAnimationTest::ImagedAnimationTest(App* app) : Test(app)
{
  app->eventManager()->addListener(this);

  PResourceFont fontResource = app->resourceManager()->resource(RESOURCE_NAME_FONT, "debug-font");
  if (fontResource)
  {
    //PTextOverlay overlay = ege_new TextOverlay(app, "lang");
    //overlay->setFont(fontResource->font());
    //app->overlayManager()->add(overlay);
    //overlay->physics()->setPosition(Vector4f(0, 80, 0));

    //overlay = ege_new TextOverlay(app, "text-1");
    //overlay->setFont(fontResource->font());
    //app->overlayManager()->add(overlay);
    //overlay->physics()->setPosition(Vector4f(40, 120, 0));

    //overlay = ege_new TextOverlay(app, "text-2");
    //overlay->setFont(fontResource->font());
    //app->overlayManager()->add(overlay);
    //overlay->physics()->setPosition(Vector4f(40, 140, 0));

    //overlay = ege_new TextOverlay(app, "text-3");
    //overlay->setFont(fontResource->font());
    //app->overlayManager()->add(overlay);
    //overlay->physics()->setPosition(Vector4f(40, 160, 0));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ImagedAnimationTest::~ImagedAnimationTest()
{
  app()->eventManager()->removeListener(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Returns test name. */
String ImagedAnimationTest::name() const
{
  return "Imaged Animation";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Initializes test. */
bool ImagedAnimationTest::initialize()
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
  if (EGE_SUCCESS != app()->resourceManager()->loadGroup("imaged-animation-test"))
  {
    // error!
    return false;
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Updates test. */
void ImagedAnimationTest::update(const Time& time)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Pointer event receiver. */
void ImagedAnimationTest::pointerEvent(PPointerData data)
{
  if (EGEInput::ACTION_BUTTON_UP == data->action())
  {
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IEventListener override. Event reciever. */
void ImagedAnimationTest::onEventRecieved(PEvent event)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
