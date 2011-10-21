#include "App.h"
#include <EGEScene.h>
#include <EGEResources.h>
#include <EGEGraphics.h>
#include <EGEInput.h>
#include <EGEEvent.h>
#include <EGEOverlay.h>
#include "Core/Components/Render/RenderComponent.h"
#include "RenderToTexture/RenderToTextureTest.h"
#include "Timeline/TimeLineTest.h"
#include "Curves/CurvesTest.h"
#include "Ripples/RipplesTest.h"
#include "Lightning/LightningTest.h"
#include "Particles/ParticleTest.h"
#include "Localization/LocalizationTest.h"

#include <gl/gl.h>
#include <gl/glu.h>

#define ORTHO 1

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
App::App() : Application(), m_activeTest(NULL)
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
bool App::start()
{
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

  if (EGE_SUCCESS != initialize(paramList))
  {
    return false;
  }

  // initialize resource manager
  resourceManager()->setRootDirectory("resources");
  if (EGE_SUCCESS != resourceManager()->addResources("resources.xml"))
  {
    // error!
    return false;
  }

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
  m_tests.push_back(new CurvesTest(this));
  m_tests.push_back(new CurvesTest(this));
  m_tests.push_back(new RipplesTest(this));
  m_tests.push_back(new LightningTest(this));
  m_tests.push_back(new LocalizationTest(this));
  m_tests.push_back(new ParticleTest(this));

  // select test to run
  selectTest();

  ege_connect(pointer(), eventSignal, this, App::pointerEvent);

  if (EGE_SUCCESS != run())
  {
    return false;
  }

  return true;
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