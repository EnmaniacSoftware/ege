#include "App.h"
#include <EGEScene.h>
#include <EGEResources.h>
#include <EGEGraphics.h>
#include <EGEInput.h>
#include <EGEEvent.h>
#include <EGEOverlay.h>
#include <EGERenderComponent.h>
#include "RenderToTexture/RenderToTextureTest.h"
#include "Timeline/TimeLineTest.h"
#include "Curves/CurvesTest.h"
#include "Ripples/RipplesTest.h"
#include "Lightning/LightningTest.h"
#include "Particles/ParticleTest.h"
#include "Localization/LocalizationTest.h"
#include "Sound/SoundTest.h"
#include "Fonts/FontsTest.h"
#include "UIScrollableView/UIScrollableViewTest.h"
#include "Dialogs/DialogTest.h"
#include "Threads/ThreadsTest.h"
#include "ImagedAnimation/ImagedAnimationTest.h"
#include "UIScrollablePageView/UIScrollablePageViewTest.h"

//#include <gl/gl.h>
//#include <gl/glu.h>

#define ORTHO 1

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(App)
EGE_DEFINE_DELETE_OPERATORS(App)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Application* Application::CreateInstance()
{
  return ege_new App();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Application::DestroyInstance(Application* instance)
{
  delete instance;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
App::App() : Application(), 
             m_activeTest(NULL)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
App::~App()
{
  for (List<Test*>::iterator it = m_tests.begin(); it != m_tests.end();)
  {
    EGE_DELETE(*it);

    m_tests.erase(it++);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult App::construct(const Dictionary& commandLineParams)
{
  EGEResult result = EGE_SUCCESS;

  Dictionary paramList;

  // setup params for render window
  paramList[EGE_RENDER_TARGET_PARAM_COLOR_BITS]   = "24";
  paramList[EGE_RENDER_TARGET_PARAM_NAME]         = EGE_PRIMARY_RENDER_TARGET_NAME;
  paramList[EGE_RENDER_TARGET_PARAM_WIDTH]        = "800";
  paramList[EGE_RENDER_TARGET_PARAM_HEIGHT]       = "600";
  paramList[EGE_RENDER_WINDOW_PARAM_FULLSCREEN]   = "false";
  paramList[EGE_RENDER_WINDOW_PARAM_DEPTH_BITS]   = "24";
  paramList[EGE_RENDER_WINDOW_PARAM_TITLE]        = "EGE Framework testbed";
  paramList[EGE_ENGINE_PARAM_UPDATES_PER_SECOND]  = "25";
  paramList[EGE_ENGINE_PARAM_LANDSCAPE_MODE]      = "true";

  // initialize engine
  if (EGE_SUCCESS != (result = Application::construct(paramList)))
  {
    // error!
    return result;
  }

  // initialize resource manager
#ifdef AIRPLAY_BUILD
  resourceManager()->addDataDirectory(".");
#else
  resourceManager()->addDataDirectory("resources");
#endif // AIRPLAY_BUILD
  if (EGE_SUCCESS != resourceManager()->addResources("resources.xml"))
  {
    // error!
    return EGE_ERROR;
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Selects test. */
bool App::selectTest()
{
  // select test to run
  m_activeTest = m_tests.back();

  return m_activeTest->initialize();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void App::update(const Time& time)
{
  // call base class
  Application::update(time);
  
  PTextOverlay overlay = overlayManager()->overlay("fps");
  if (overlay)
  {
    overlay->setText(Text::Format("FPS:%d", fps()));
  }

  overlay = overlayManager()->overlay("info");
  if (overlay && m_activeTest)
  {
    overlay->setText(Text::Format("%s", m_activeTest->name().toAscii()));
  }

  if (m_activeTest)
  {
    m_activeTest->update(time);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Pointer event receiver. */
void App::pointerEvent(PPointerData data)
{
  if (m_activeTest)
  {
    m_activeTest->pointerEvent(data);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Version App::version() const
{
  return Version(0, 1, 1);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void App::onInitialized()
{
  PResourceFont fontResource = resourceManager()->resource(RESOURCE_NAME_FONT, "debug-font");
  if (fontResource)
  {
    PTextOverlay overlay = ege_new TextOverlay(this, "fps");
    overlay->setFont(fontResource->font());
    overlayManager()->add(overlay);
    overlay->physics()->setPosition(Vector4f(0, 20, 0));

    overlay = ege_new TextOverlay(this, "info");
    overlay->setFont(fontResource->font());
    overlayManager()->add(overlay);
  }

  // create tests
  m_tests.push_back(new RenderToTextureTest(this));
  m_tests.push_back(new TimeLineTest(this));
  m_tests.push_back(new RipplesTest(this));
  m_tests.push_back(new ParticleTest(this));
  m_tests.push_back(new SoundTest(this));
  m_tests.push_back(new FontsTest(this));
  m_tests.push_back(new DialogTest(this));
  m_tests.push_back(new LightningTest(this));
  m_tests.push_back(new ThreadsTest(this));
  m_tests.push_back(new LocalizationTest(this));
  m_tests.push_back(new ImagedAnimationTest(this));
  m_tests.push_back(new UIScrollableViewTest(this));
  m_tests.push_back(new UIScrollablePageViewTest(this));
  m_tests.push_back(new CurvesTest(this));

  // select test to run
  selectTest();

  ege_connect(pointer(), eventSignal, this, App::pointerEvent);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
