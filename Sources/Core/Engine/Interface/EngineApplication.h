#ifndef EGE_CORE_ENGINE_ENGINEAPPLICATION_H
#define EGE_CORE_ENGINE_ENGINEAPPLICATION_H

/*! Public interface for clients running engine.
 */

#include "EGE.h"
#include "EGEVersion.h"
#include "EGEDictionary.h"
#include "EGETime.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Engine;
class PointerEvent;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class EngineApplication
{
  public:

    EngineApplication(Engine& engine);
    virtual ~EngineApplication();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! Creates an application instance.
     *  @param  engine  Reference to engine object associated with the application.
     *  @return Allocated application object.
     *  @note This is to be provided by application consuming EGE library.
     */
    static EngineApplication* CreateInstance(Engine& engine);
    /*! Destroys the application instance. This is to be provided by application consuming EGE library.
     *  @param  instance Instance of the application to destroy.
     *  @note This is to be provided by application consuming EGE library.
     *        Instance being destroyed should be previously created by @ref EngineApplication::CreateInstance.
     */
    static void DestroyInstance(EngineApplication* instance);

  public:

    /*! Returns reference to engine. */
    Engine& engine() const;

    /*! Constructs object. 
     *  @return EGE_SUCCESS on success. Otherwise, one of available error codes.
     */
    virtual EGEResult construct() = 0;
    /*! Returns configuration dictionary for engine initialization. 
     *  @note This method is called before engine initialization.
     */
    virtual Dictionary engineConfiguration() = 0;
    /*! Updates application.
     *  @param  time  Time increment.
     */
    virtual void update(const EGE::Time& time) = 0;
    /*! Returns application version. */
    virtual Version version() const = 0;

    /*! Requests quit. */
    virtual void close();

    /*! Called when application is about to be suspended. */
    virtual void onSuspend() = 0;
    /*! Called when application is about to be resumed. */
    virtual void onResume() = 0;
    
    /*! Called when application recieves a pointer event.
     *  @param  event Pointer event.
     */
    virtual void onPointerEvent(const PointerEvent& event) = 0;

  private:

    /*! Reference to engine. */
    Engine& m_engine;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_ENGINE_ENGINEAPPLICATION_H