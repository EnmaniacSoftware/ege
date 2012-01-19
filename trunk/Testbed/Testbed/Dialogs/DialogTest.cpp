#include "App.h"
#include "DialogTest.h"
#include <EGESignal.h>
#include <EGEResources.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#define ORTHO

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DialogTest::DialogTest(App* app) : Test(app)
{
  ege_connect(app->graphics(), preRender, this, DialogTest::preRender);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DialogTest::~DialogTest()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Returns test name. */
String DialogTest::name() const
{
  return "Dialogs";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Initializes test. */
bool DialogTest::initialize()
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
  if (EGE_SUCCESS != app()->resourceManager()->loadGroup("dialogs-test"))
  {
    // error!
    return false;
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Updates test. */
void DialogTest::update(const Time& time)
{
  if (m_dialog)
  {
    m_dialog->update(time);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Pointer event receiver. */
void DialogTest::pointerEvent(PPointerData data)
{
  if (m_dialog)
  {
    m_dialog->pointerEvent(data);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Slot called when resource group has been loaded. */
void DialogTest::groupLoadComplete(const String& name)
{
  if ("dialogs-test" == name)
  {
    PResourceDialog dialogRes = app()->resourceManager()->resource(RESOURCE_NAME_DIALOG, "my-dialog");
    EGE_ASSERT(dialogRes);

    m_dialog = dialogRes->createInstance();

    m_dialog->physics().setPosition(Vector4f(100, 100, 0));

    PResourceFont fontResource = app()->resourceManager()->resource(RESOURCE_NAME_FONT, "debug-font");
    if (fontResource)
    {
      PTextOverlay text = ege_new TextOverlay(app(), "text");
      text->setFont(fontResource->font());
      text->setText(Text::Format("Title"));
      text->physics()->setPosition(Vector4f(0, 0, 0));
      m_dialog->addTextOverlay("title", text);

      // add scroll area content
      for (s32 i = 0; i < 10; ++i)
      {
        text = ege_new TextOverlay(app(), "text");
        text->setFont(fontResource->font());
        text->setText(Text::Format("Content line %d Content line", i + 1));
        text->physics()->setPosition(Vector4f(0, i * 50.0f, 0));
        m_dialog->addTextOverlay("content", text);
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called before target is rendered. */
void DialogTest::preRender(PRenderTarget target)
{
  EGE_UNUSED(target);

  if (m_dialog)
  {
    m_dialog->addForRendering(app()->graphics()->renderer());
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------