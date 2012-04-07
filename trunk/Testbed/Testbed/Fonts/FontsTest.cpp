#include "App.h"
#include "FontsTest.h"
#include <EGESignal.h>
#include <EGEResources.h>
#include <EGEOverlay.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#define ORTHO

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
FontsTest::FontsTest(App* app) : Test(app)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
FontsTest::~FontsTest()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Returns test name. */
String FontsTest::name() const
{
  return "Fonts";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Initializes test. */
bool FontsTest::initialize()
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
  if ((EGE_SUCCESS != app()->resourceManager()->loadGroup("fonts-test")) ||
      (EGE_SUCCESS != app()->resourceManager()->loadGroup("fonts")))
  {
    // error!
    return false;
  }

  app()->eventManager()->addListener(this);

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Updates test. */
void FontsTest::update(const Time& time)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IEventListener override. Event reciever. */
void FontsTest::onEventRecieved(PEvent event)
{
  if (EGE_EVENT_ID_CORE_LANGUAGE_CHANGED == event->uid())
  {
    updateTexts();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates texts. */
void FontsTest::updateTexts()
{
  ege_cast<TextOverlay*>(app()->overlayManager()->overlay("fonts-text-1"))->setText(TR(app(), "test-text-1"));
  ege_cast<TextOverlay*>(app()->overlayManager()->overlay("fonts-text-2"))->setText(TR(app(), "test-text-2"));
  ege_cast<TextOverlay*>(app()->overlayManager()->overlay("fonts-text-3"))->setText(TR(app(), "test-text-3"));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Slot called when resource group has been loaded. */
void FontsTest::groupLoadComplete(const String& name)
{
  if ("fonts-test" == name)
  {
  }
  else if ("fonts" == name)
  {
    PResourceFont fontResource = app()->resourceManager()->resource(RESOURCE_NAME_FONT, "fonts");
    if (fontResource)
    {
      PTextOverlay overlay = ege_new TextOverlay(app(), "fonts-text-1");
      overlay->setFont(fontResource->font());
      app()->overlayManager()->add(overlay);
      overlay->physics()->setPosition(Vector4f(10, 120, 0));

      overlay = ege_new TextOverlay(app(), "fonts-text-2");
      overlay->setFont(fontResource->font());
      app()->overlayManager()->add(overlay);
      overlay->physics()->setPosition(Vector4f(10, 180, 0));

      overlay = ege_new TextOverlay(app(), "fonts-text-3");
      overlay->setFont(fontResource->font());
      app()->overlayManager()->add(overlay);
      overlay->physics()->setPosition(Vector4f(10, 240, 0));
    }

    updateTexts();  
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
