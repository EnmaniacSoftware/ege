#include "Core/Application/AppController.h"
#include "Core/Application/Application.h"
#include "Core/Physics/PhysicsManager.h"
#include "Core/Scene/SceneManager.h"
#include "Core/Graphics/Render/RenderWindow.h"
#include "Core/Event/EventManager.h"
#include "Core/Event/Event.h"
#include "Core/Event/EventIDs.h"
#include "Core/Overlay/OverlayManager.h"
#include "Core/Screen/ScreenManager.h"
#include "Core/Audio/AudioManager.h"
#include "Core/Resource/ResourceManager.h"
#include <EGEGraphics.h>
#include <EGETimer.h>

#ifdef EGE_PLATFORM_WIN32
#include "Win32/Application/AppControllerWin32_p.h"
#elif EGE_PLATFORM_AIRPLAY
#include "Airplay/Application/AppControllerAirplay_p.h"
#endif

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(AppController)
EGE_DEFINE_DELETE_OPERATORS(AppController)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AppController::AppController(Application* app, const Dictionary& params) : Object(app), 
                                                                           m_state(STATE_RUNNING), 
                                                                           m_fps(0), 
                                                                           m_rendersCount(0)
{
  // decompose param list
  Dictionary::const_iterator iterUPS = params.find(EGE_ENGINE_PARAM_UPDATES_PER_SECOND);
  Dictionary::const_iterator iterFPS = params.find(EGE_ENGINE_PARAM_RENDERS_PER_SECOND);

  m_updateInterval.fromMiliseconds((iterUPS != params.end()) ? 1000 / iterUPS->second.toInt() : 0);
  m_renderInterval.fromMiliseconds((iterFPS != params.end()) ? 1000 / iterFPS->second.toInt() : 0);

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
  // initialize update timer (to smooth out first update)
  m_lastUpdateTime.fromMicroseconds(Timer::GetMicroseconds());

  return p_func()->run();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IEventListener override. Event reciever. */
void AppController::onEventRecieved(PEvent pEvent)
{
  switch (pEvent->id())
  {
    case EGE_EVENT_ID_CORE_QUIT_REQUEST:

      m_state = STATE_QUITTING;
      // TAGE - for testing
      //m_state = STATE_QUIT;
      break;

    case EGE_EVENT_ID_CORE_APP_PAUSE:

      if (STATE_RUNNING == m_state)
      {
        m_state = STATE_PAUSED;
      }
      break;

    case EGE_EVENT_ID_CORE_APP_RESUME:

      if (STATE_PAUSED == m_state)
      {
        m_state = STATE_RUNNING;
      }
      break;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates application. */
void AppController::update()
{
  // get current time
  Time time(Timer::GetMicroseconds());

  // calculate time passed
  Time timeInterval = time - m_lastUpdateTime;

  // dont allow to long changes
  if (0.25f < timeInterval.seconds())
  {
    timeInterval = 0.25f;
  }

  // store frame current time
  m_lastUpdateTime = time;

  // check if quitting
  if (STATE_QUITTING == m_state)
  {
    // update only object which needs time to shut down
    app()->resourceManager()->update(m_updateInterval);
    app()->audioManager()->update(m_updateInterval);

    // check if ready to quit
    if ((ResourceManager::STATE_CLOSED == app()->resourceManager()->state()) &&
        (AudioManager::STATE_CLOSED == app()->audioManager()->state()))
    {
      // change state
      m_state = STATE_QUIT;
    }
  }
  else if (STATE_RUNNING == m_state)
  {
    // update accumulator for updates
    m_updateAccumulator += timeInterval;

    // update as much as requested
    while (m_updateAccumulator > m_updateInterval)
    {
      m_updateAccumulator -= m_updateInterval;
      app()->physicsManager()->update(m_updateInterval);
    }

    app()->resourceManager()->update(timeInterval);
    app()->audioManager()->update(timeInterval);
    app()->screenManager()->update(timeInterval);
    app()->sceneManager()->update(timeInterval);
    app()->overlayManager()->update(timeInterval);
    app()->update(timeInterval);

    // interpolate physics by remaining value
    // ..
  }

  // store update duration
  m_lastFrameUpdateDuration.fromMicroseconds(Timer::GetMicroseconds() - m_lastUpdateTime.microseconds());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Renders application. */
void AppController::render()
{
  // get current time
  Time time(Timer::GetMicroseconds());

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

  if (STATE_RUNNING == m_state)
  {
    // do render
    app()->graphics()->render();
  }

  // store render duration
  m_lastFrameRenderDuration.fromMicroseconds(Timer::GetMicroseconds() - time.microseconds());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool AppController::isValid() const
{
  return (NULL != m_p);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END