#ifndef EGE_CORE_APPLICATION_H
#define EGE_CORE_APPLICATION_H

#include "EGE.h"
#include "Core/Event/EventListener.h"
#include "Core/ConfigParams.h"
#include "Core/Timer/Time.h"
#include "Core/Scene/SceneManager.h"
#include "Core/Overlay/OverlayManager.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DECLARE_SMART_CLASS(Timer, PTimer)
EGE_DECLARE_SMART_CLASS(Graphics, PGraphics)
EGE_DECLARE_SMART_CLASS(EventManager, PEventManager)
EGE_DECLARE_SMART_CLASS(ResourceManager, PResourceManager)
EGE_DECLARE_SMART_CLASS(RenderTarget, PRenderTarget)
EGE_DECLARE_SMART_CLASS(PhysicsManager, PPhysicsManager)
EGE_DECLARE_SMART_CLASS(AppController, PAppController)
EGE_DECLARE_SMART_CLASS(Pointer, PPointer)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Application : public IEventListener
{
  public:

    Application();
    virtual ~Application();

    /** Initializes engine.
    *  
    *   \param  params    list of parameters to initialize engine with
    *   \param  listener  listener object which is to be notified with engine events. 
    */
    virtual EGEResult initialize(const ConfigParams& params);
    /* Starts engine work. */
    virtual EGEResult run();
    /* Returns graphics subsystem object. */
    PGraphics graphics() const;
    /* Returns main timer. */
    PTimer timer() const;
    /* Returns event manager. */
    PEventManager eventManager() const;
    /* Returns physics manager. */
    PPhysicsManager physicsManager() const;
    /* Returns scene manager. */
    PSceneManager sceneManager() const;
    /* Returns resource manager. */
    PResourceManager resourceManager() const;
    /* Returns pointer input. */
    PPointer pointer() const;
    /* Returns overlay manager. */
    POverlayManager overlayManager() const;
    /*! Returns TRUE if landscape mode is enabled. */
    inline bool isLandscape() const { return m_landscapeMode; }
    /* Returns current FPS indication. */
    s32 fps() const;

  protected:

    /* Application updater. */
    virtual void update(const Time& time);
    /* IEventListener override. Event reciever. */
    virtual void onEventRecieved(PEvent event);

  private:

    /* Returns application controller. */
    PAppController appController() const;

  private:

    /*! Scene manager. */
    PSceneManager m_sceneManager;
    /*! Physics manager. */
    PPhysicsManager m_physicsManager;
    /*! Event manager. */
    PEventManager m_eventManager;
    /*! Graphics subsystem obejct. */
    PGraphics m_graphics;
    /*! Application controller. */
    PAppController m_appController;
    /*! Main timer object. */
    // TAGE - potrzebny ?
    PTimer m_timer;
    /*! Resource manager. */
    PResourceManager m_resourceManager;
    /*! Pointer input. */
    PPointer m_pointer;
    /*! Overlay manager. */
    POverlayManager m_overlayManager;
    /*! Landscape mode flag. */
    bool m_landscapeMode;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_APPLICATION_H