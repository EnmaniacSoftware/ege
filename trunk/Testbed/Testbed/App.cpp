#include "App.h"
#include <EGEScene.h>
#include <EGEResources.h>
#include <EGEGraphics.h>
#include <EGEInput.h>
#include <EGEEvent.h>
#include <EGEOverlay.h>
#include "Core/Components/Render/RenderComponent.h"
#include "RenderToTextureTest.h"
#include "TimeLineTest.h"
#include "CurvesTest.h"

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
  ConfigParams paramList;

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

  // create tests
  m_tests.push_back(new RenderToTextureTest(this));
  m_tests.push_back(new TimeLineTest(this));
  m_tests.push_back(new CurvesTest(this));

  // select test to run
  selectTest();

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
  if (m_activeTest)
  {
    m_activeTest->update(time);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*bool App::createScene()
{
#ifdef EGE_PLATFORM_WIN32
  resourceManager()->setRootDirectory("../Airplay/data/resources");
#else
  resourceManager()->setRootDirectory("resources");
#endif // EGE_PLATFORM_WIN32

  if (EGE_SUCCESS != resourceManager()->addResources("resources.xml"))
  {
    return false;
  }

  resourceManager()->loadGroup("fonts");

  PTextOverlay overlay = overlayManager()->addTextOverlay("debug");
  PResourceFont fontResource = resourceManager()->resource("font", "ingame");
  overlay->setFont(fontResource->font());

  overlay->setText(Text::Format(""));
  overlay->physics()->setPosition(Vector4f(0, 20, 0, 0));

    // get window render target (created thru config options)
  PRenderWindow window = graphics()->renderTarget(EGE_PRIMARY_RENDER_TARGET_NAME);

  // create new camera
  PCamera camera = ege_new Camera("myCamera", sceneManager());

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

  m_obj[0] = addObject("first", Color::RED);
  m_obj[1] = addObject("second", Color::GREEN);

  m_spline[0].setType(CubicSpline::TYPE_BEZIER);
  m_spline[0].addPoint(Vector4f(100, 100, 0), Vector4f(150, 100, 0));
  m_spline[0].addPoint(Vector4f(300, 100, 0), Vector4f(250, 100, 0));
  m_spline[0].addPoint(Vector4f(300, 200, 0), Vector4f(300, 150, 0)).setBeginTangent(Vector4f(300, 150, 0));

  float32 length = m_spline[0].length();

  //m_spline[1].setType(CubicSpline::TYPE_HERMITE);
  //m_spline[1].addControlPoint(Vector4f(100, 400, 0), Vector4f(150, 350, 0));
  //m_spline[1].addControlPoint(Vector4f(500, 350, 0), Vector4f(550, 400, 0));

  //m_spline[1].addControlPoint(3.0f * (Vector4f(150, 350, 0) - Vector4f(100, 400, 0)));
  //m_spline[1].addControlPoint(3.0f * (Vector4f(500, 350, 0) - Vector4f(550, 400, 0)));

  //m_spline[2].setType(CubicSpline::TYPE_CARDINAL);
  //m_spline[2].addControlPoint(Vector4f(100, 400, 0), Vector4f(150, 350, 0));
  //m_spline[2].addControlPoint(Vector4f(500, 350, 0), Vector4f(550, 400, 0));

  //m_spline[3].setType(CubicSpline::TYPE_BSPLINE);
  //m_spline[3].addControlPoint(Vector4f(100, 400, 0), Vector4f(150, 350, 0));
  //m_spline[3].addControlPoint(Vector4f(500, 350, 0), Vector4f(550, 400, 0));

  return true;
}*/
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//RenderObject* App::addObject(const String& name, const Color& color)
//{
//  RenderObject* obj = ege_new RenderObject(this, name);
//  obj->setColor(color);
//
//  sceneManager()->rootNode()->createChildSceneNode(name)->attachObject(obj);
//
//  return obj;
//}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
