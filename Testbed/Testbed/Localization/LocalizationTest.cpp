#include "App.h"
#include "LocalizationTest.h"
#include <EGESignal.h>
#include <EGEResources.h>
#include <EGEOverlay.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#define ORTHO

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
LocalizationTest::LocalizationTest(App* app) : Test(app)
{
  app->eventManager()->addListener(this);

  PResourceFont fontResource = app->resourceManager()->resource(RESOURCE_NAME_FONT, "debug-font");
  if (fontResource)
  {
    PTextOverlay overlay = ege_new TextOverlay(app, "lang");
    overlay->setFont(fontResource->font());
    app->overlayManager()->add(overlay);
    overlay->physics()->setPosition(Vector4f(0, 80, 0));

    overlay = ege_new TextOverlay(app, "text-1");
    overlay->setFont(fontResource->font());
    app->overlayManager()->add(overlay);
    overlay->physics()->setPosition(Vector4f(40, 120, 0));

    overlay = ege_new TextOverlay(app, "text-2");
    overlay->setFont(fontResource->font());
    app->overlayManager()->add(overlay);
    overlay->physics()->setPosition(Vector4f(40, 140, 0));

    overlay = ege_new TextOverlay(app, "text-3");
    overlay->setFont(fontResource->font());
    app->overlayManager()->add(overlay);
    overlay->physics()->setPosition(Vector4f(40, 160, 0));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
LocalizationTest::~LocalizationTest()
{
  app()->eventManager()->removeListener(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Returns test name. */
String LocalizationTest::name() const
{
  return "Localization";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Initializes test. */
bool LocalizationTest::initialize()
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
  if (EGE_SUCCESS != app()->resourceManager()->loadGroup("localization-test"))
  {
    // error!
    return false;
  }

  updateTexts();

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Updates test. */
void LocalizationTest::update(const Time& time)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Pointer event receiver. */
void LocalizationTest::pointerEvent(PPointerData data)
{
  if (ACTION_BUTTON_UP == data->action())
  {
    if ("en" == app()->language())
    {
      app()->setLanguage("pl");
    }
    else
    {
      app()->setLanguage("en");
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IEventListener override. Event reciever. */
void LocalizationTest::onEventRecieved(PEvent event)
{
  if (EGE_EVENT_ID_CORE_LANGUAGE_CHANGED == event->uid())
  {
    updateTexts();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates texts. */
void LocalizationTest::updateTexts()
{
  ege_cast<TextOverlay*>(app()->overlayManager()->overlay("lang"))->setText(Text::Format("Language: %s", app()->language().toAscii()));

  ege_cast<TextOverlay*>(app()->overlayManager()->overlay("text-1"))->setText(TR(app(), "text-1"));
  ege_cast<TextOverlay*>(app()->overlayManager()->overlay("text-2"))->setText(TR(app(), "text-2"));
  ege_cast<TextOverlay*>(app()->overlayManager()->overlay("text-3"))->setText(TRN(app(), "text-3", 2));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
