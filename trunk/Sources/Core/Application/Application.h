#ifndef EGE_CORE_APPLICATION_H
#define EGE_CORE_APPLICATION_H

#include <EGE.h>
#include <EGETime.h>
#include "Core/Event/EventListener.h"
#include "Core/ConfigParams.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class ScreenManager;
class Graphics;
class EventManager;
class ResourceManager;
class PhysicsManager;
class AppController;
class Pointer;
class SceneManager;
class OverlayManager;
class Debug;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Application
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
    /* Application updater. */
    virtual void update(const Time& time);
    /*! Returns graphics subsystem object. */
    inline Graphics* graphics() const { return m_graphics; }
    /*! Returns event manager. */
    EventManager* eventManager() const { return m_eventManager; }
    /*! Returns physics manager. */
    PhysicsManager* physicsManager() const { return m_physicsManager; }
    /*! Returns scene manager. */
    SceneManager* sceneManager() const { return m_sceneManager; }
    /*! Returns resource manager. */
    ResourceManager* resourceManager() const { return m_resourceManager; }
    /*! Returns pointer input. */
    Pointer* pointer() const { return m_pointer; }
    /*! Returns overlay manager. */
    OverlayManager* overlayManager() const { return m_overlayManager; }
    /*! Returns screen manager. */
    ScreenManager* screenManager() const { return m_screenManager; }
    /*! Returns debug object. */
    Debug* debug() const { return m_debug; }
    /*! Returns TRUE if landscape mode is enabled. */
    inline bool isLandscape() const { return m_landscapeMode; }
    /* Returns current FPS indication. */
    s32 fps() const;

  private:

    /*! Returns application controller. */
    AppController* appController() const { return m_appController; }

  private:

    /*! Scene manager. */
    SceneManager* m_sceneManager;
    /*! Physics manager. */
    PhysicsManager* m_physicsManager;
    /*! Event manager. */
    EventManager* m_eventManager;
    /*! Graphics subsystem obejct. */
    Graphics* m_graphics;
    /*! Application controller. */
    AppController* m_appController;
    /*! Resource manager. */
    ResourceManager* m_resourceManager;
    /*! Pointer input. */
    Pointer* m_pointer;
    /*! Overlay manager. */
    OverlayManager* m_overlayManager;
    /*! Screen manager. */
    ScreenManager* m_screenManager;
    /*! Debug object. */
    Debug* m_debug;
    /*! Landscape mode flag. */
    bool m_landscapeMode;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_APPLICATION_H