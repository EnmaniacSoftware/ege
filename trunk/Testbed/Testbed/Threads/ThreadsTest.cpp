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
  for (s32 i = 0; i < THREADS_COUNT; i++)
  {
    m_counters[i] = i * 1000;
    m_threads[i] = ege_new TestThread(app, &m_counters[i]);
    ege_connect(m_threads[i], started, this, ThreadsTest::onThreadStarted);
    ege_connect(m_threads[i], finished, this, ThreadsTest::onThreadFinished);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ThreadsTest::~ThreadsTest()
{
  for (s32 i = 0; i < THREADS_COUNT; i++)
  {
    ege_disconnect(m_threads[i], started, this, ThreadsTest::onThreadStarted);
    ege_disconnect(m_threads[i], finished, this, ThreadsTest::onThreadFinished);
    m_threads[i] = NULL;
  }
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
  for (s32 i = 0; i < THREADS_COUNT; ++i)
  {
    PTextOverlay overlay = app()->overlayManager()->overlay(String::Format("thread-%d-counter", i));
    overlay->setText(Text::Format("Thread %d: %d r: %d f: %d", i, m_counters[i], m_threads[i]->isRunning(), m_threads[i]->isFinished()));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Test override. Pointer event receiver. */
void ThreadsTest::pointerEvent(PPointerData data)
{
  if (ACTION_BUTTON_UP == data->action())
  {
    for (s32 i = 0; i < THREADS_COUNT; ++i)
    {
      if (!m_threads[i]->isRunning())
      {
        m_threads[i]->start();
      }
      else
      {
        m_threads[i]->stop();
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called then thread has finished. */
void ThreadsTest::onThreadFinished(const Thread* thread)
{
  EGE_PRINT("Thread finished");
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called then thread has started. */
void ThreadsTest::onThreadStarted(const Thread* thread)
{
  EGE_PRINT("Thread Started");
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
