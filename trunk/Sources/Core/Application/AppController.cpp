#include "Core/Application/AppController.h"
#include "Core/Application/Application.h"
#include "Core/String/StringUtils.h"
#include "Core/Physics/PhysicsManager.h"
#include "Core/Scene/SceneManager.h"
#include "Core/Graphics/Render/RenderWindow.h"
#include "Core/Event/EventManager.h"
#include "Core/Event/Event.h"
#include "Core/Event/EventIDs.h"
#include "EGEGraphics.h"
#include "EGETimer.h"

#ifdef EGE_PLATFORM_WIN32
#include "Win32/Application/AppControllerWin32_p.h"
#elif EGE_PLATFORM_AIRPLAY
#include "Airplay/Application/AppControllerAirplay_p.h"
#endif

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(AppController)
EGE_DEFINE_DELETE_OPERATORS(AppController)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AppController::AppController(Application* app, const ConfigParams& params) : Object(app), m_state(STATE_RUNNING), m_fps(0), m_rendersCount(0)
{
  // decompose param list
  ConfigParams::const_iterator iterUPS = params.find(EGE_ENGINE_PARAM_UPDATES_PER_SECOND);
  m_updateInterval.fromMiliseconds((iterUPS != params.end()) ? 1000 / StringUtils::ToU32(iterUPS->second) : 0);

  // subscribe for event notifications
  if (app->eventManager()->addListener(this))
  {
    m_p = ege_new AppControllerPrivate(this);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AppController::~AppController()
{
  EGE_DELETE(m_p);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Enters main loop. */
EGEResult AppController::run()
{
  // get current time
  PTimer timer = app()->timer();

  // initialize update timer (to smooth out first update)
  m_lastUpdateTime.fromMicroseconds(timer->microseconds());

  return p_func()->run();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IEventListener override. Event reciever. */
void AppController::onEventRecieved(PEvent pEvent)
{
  switch (pEvent->uid())
  {
    case EGE_EVENT_UID_CORE_QUIT_REQUEST:

      // TAGE - for testing
      m_state = STATE_QUIT;
      break;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates application. */
void AppController::update()
{
  // get current time
  PTimer timer = app()->timer();
  Time time(timer->microseconds());

  // check if update is to be made
  if (time - m_lastUpdateTime > m_updateInterval)
  {
    // get time interval
    Time timeInterval = time - m_lastUpdateTime;

    // update physics
    app()->physicsManager()->update(timeInterval);

    // update scene
    app()->sceneManager()->update(timeInterval);

    // update overlays
    app()->overlayManager()->update(timeInterval);

    // send event
    app()->eventManager()->send(EGE_EVENT_UID_CORE_UPDATE, timeInterval.seconds());

    // store frame current time
    m_lastUpdateTime = time;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Renders application. */
void AppController::render()
{
  // get current time
  PTimer timer = app()->timer();
  Time time(timer->microseconds());

  // check if 1 second hasnt passed yet
  if (time - m_fpsCountStartTime < Time(1.0f))
  {
    // new render
    ++m_rendersCount;
  }
  else
  {
    // store FPS indication
    m_fps = m_rendersCount;

    // reset renders count
    m_rendersCount = 0;

    // reset time stamp
    m_fpsCountStartTime = time;
  }

  // do render
  app()->graphics()->render();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool AppController::isValid() const
{
  return NULL != m_p;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
