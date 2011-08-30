#include "App.h"
#include "TimeLineTest.h"
#include <EGESignal.h>
#include <EGEDebug.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TimeLineTest::TimeLineTest(App* app) : Test(app)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TimeLineTest::~TimeLineTest()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Returns test name. */
String TimeLineTest::name() const
{
  return "Timeline";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Creates scene. */
bool TimeLineTest::createScene()
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

  m_timeLine = ege_new TimeLine(app());
  m_timeLine->setStartFrame(0);
  m_timeLine->setEndFrame(10);
  m_timeLine->setDuration(Time::FromSeconds(10.0f));
  
  ege_connect(m_timeLine, frameChanged, this, TimeLineTest::frameChanged);
  ege_connect(m_timeLine, finished, this, TimeLineTest::finished);

  m_timeLine->start();

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Updates test. */
void TimeLineTest::update(const Time& time)
{
  m_timeLine->update(time);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when time line frame has been changed. */
void TimeLineTest::frameChanged(s32 frame)
{
  EGE_PRINT("Frame changed to: %d", frame);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when time line finishes. */
void TimeLineTest::finished()
{
  EGE_PRINT("Done!");
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
