#ifndef EGE_CORE_APPCONTROLLER_H
#define EGE_CORE_APPCONTROLLER_H

#include <EGE.h>
#include <EGETime.h>
#include <EGETimer.h>
#include "Core/Event/EventListener.h"
#include <EGEDictionary.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(AppController, PAppController)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class AppController : public Object, public IEventListener
{
  public:

    AppController(Application* app, const Dictionary& params);
    virtual ~AppController();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! Available states. */
    enum State
    {
      STATE_INVALID = 0,
      STATE_RUNNING,
      STATE_PAUSED,
      STATE_QUITTING,
      STATE_QUIT
    };

  public:

    /*! Creates object. */
    EGEResult construct();
    /*! Initializes controller with given parameters. */
    EGEResult initialize(const Dictionary& mParams);
    /*! Enters main loop. */
    EGEResult run();
    /*! Updates application. */
    void update();
    /*! Renders application. */
    void render();
    /*! Returns FPS indication. */
    s32 fps() const { return m_fps; }
    /*! Returns controller timer. */
    const Timer& timer() const { return m_timer; }
    /*! Returns current state. */
    State state() const { return m_state; }
    /*! Returns last frame update duration. */
    const Time& lastFrameUpdateDuration() const { return m_lastFrameUpdateDuration; }
    /*! Returns last frame render duration. */
    const Time& lastFrameRenderDuration() const { return m_lastFrameRenderDuration; }

  private:

    /*! IEventListener override. Event reciever. */
    void onEventRecieved(PEvent pEvent) override;
   
  private:
    
    EGE_DECLARE_PRIVATE_IMPLEMENTATION(AppController);

    ///*! Time at which last update was done. */
    //Time m_lastUpdateTime;
    ///*! Time interval between updates. */
    //Time m_updateInterval;
    ///*! Time interval between renders. */
    //Time m_renderInterval;
    ///*! Currently accumulated update duration. */
    //Time m_updateAccumulator;
    ///*! Current state. */
    //State m_state;
    ///*! Current FPS indication. */
    //s32 m_fps;
    ///*! Number of frames rendering within current count interval. */
    //s32 m_rendersCount;
    ///*! Time at which last FPS count started. */
    //Time m_fpsCountStartTime;
    ///*! Timer. */
    //Timer m_timer;
    ///*! Last frame update duration. */
    //Time m_lastFrameUpdateDuration;
    ///*! Last frame render duration. */
    //Time m_lastFrameRenderDuration;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_APPCONTROLLER_H