#include "App.h"
#include "SoundTest.h"
#include <EGESignal.h>
#include <EGEResources.h>
#include <EGEOverlay.h>
#include <EGEAudio.h>
#include <EGEDevice.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#define ORTHO

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SoundTest::SoundTest(App* app) : Test(app)
{
  PResourceFont fontResource = app->resourceManager()->resource(RESOURCE_NAME_FONT, "debug-font");
  if (fontResource)
  {
    PTextOverlay overlay = ege_new TextOverlay(app, "sound-text");
    overlay->setFont(fontResource->font());
    app->overlayManager()->add(overlay);
    overlay->physics()->setPosition(Vector4f(0, 80, 0));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SoundTest::~SoundTest()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Returns test name. */
String SoundTest::name() const
{
  return "Sound";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Initializes test. */
bool SoundTest::initialize()
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

  PResourceFont fontResource = app()->resourceManager()->resource(RESOURCE_NAME_FONT, "debug-font");
  if (fontResource)
  {
    PTextOverlay overlay = ege_new TextOverlay(app(), "frequency");
    overlay->setFont(fontResource->font());
    app()->overlayManager()->add(overlay);
    overlay->physics()->setPosition(Vector4f(0, 60, 0));
    overlay->setText(Text::Format("Output Freq: %dHz", Device::AudioOutputFrequency()));

    overlay = ege_new TextOverlay(app(), "sound-info");
    overlay->setFont(fontResource->font());
    app()->overlayManager()->add(overlay);
    overlay->physics()->setPosition(Vector4f(0, 80, 0));
  }

  // load resources
  if (EGE_SUCCESS != app()->resourceManager()->loadGroup("sound-test"))
  {
    // error!
    return false;
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Updates test. */
void SoundTest::update(const Time& time)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Pointer event receiver. */
void SoundTest::pointerEvent(PPointerData data)
{
  if (EGEInput::ACTION_BUTTON_DOWN == data->action() && EGEInput::BUTTON_LEFT == data->button())
  {
    // get window render target (created thru config options)
    PRenderWindow window = app()->graphics()->renderTarget(EGE_PRIMARY_RENDER_TARGET_NAME);

    String name;

    if (data->x() < (window->width() >> 1))
    {
      if (data->y() < (window->height() >> 1))
      {
        name = "mono-22050";
      }
      else
      {
        name = "stereo-22050";
      }
    }
    else
    {
      if (data->y() < (window->height() >> 1))
      {
        name = "mono-11025";
      }
      else
      {
        name = "stereo-11025";
      }
    }

    PResourceSound soundRes = app()->resourceManager()->soundResource(name);
    if (soundRes)
    {
      PSound sound = soundRes->createInstance();

      PTextOverlay overlay = app()->overlayManager()->overlay("sound-info");
      if (overlay)
      {
        overlay->setText(Text::Format("Playing %s. Channels: %d. Freq: %dHz", name.toAscii(), sound->codec()->channels(), sound->codec()->frequency()));
      }

      app()->audioManager()->play(sound);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
