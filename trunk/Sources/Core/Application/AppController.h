#ifndef EGE_CORE_APPCONTROLLER_H
#define EGE_CORE_APPCONTROLLER_H

#include "EGE.h"
#include "Core/Event/EventListener.h"
#include "Core/ConfigParams.h"
#include "EGETime.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DECLARE_SMART_CLASS(AppController, PAppController)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class AppController : public Object, public IEventListener
{
  public:

    AppController(Application* app, const ConfigParams& params);
    virtual ~AppController();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /* Initializes controller with given parameters. */
    EGEResult initialize(const ConfigParams& mParams);
    /* Enters main loop. */
    EGEResult run();
    /* Updates application. */
    void update();
    /* Renders application. */
    void render();

  private:

    enum State
    {
      STATE_RUNNING,
      STATE_QUITTING,
      STATE_QUIT
    };

  private:

    /* IEventListener override. Event reciever. */
    void onEventRecieved(PEvent pEvent) override;
    /*! Returns current state. */
    inline State state() const { return m_state; }
   
  private:
    
    EGE_DECLARE_PRIVATE_IMPLEMENTATION(AppController);

    /*! Time at which last update was done. */
    Time m_lastUpdateTime;
    /*! Time interval between updates. */
    Time m_updateInterval;
    /*! Current state. */
    State m_state;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_APPCONTROLLER_H