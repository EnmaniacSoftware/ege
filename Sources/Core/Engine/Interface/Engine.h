#ifndef EGE_CORE_ENGINE_ENGINE_H
#define EGE_CORE_ENGINE_ENGINE_H

/*! Public interface of an engine implementation.  
 */

#include "EGE.h"
#include "EGEDictionary.h"
#include "EGESignal.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class IScreenManager;
class IGraphics;
class IEventManager;
class IResourceManager;
class IPhysicsManager;
class Pointer;
class ISceneManager;
class IOverlayManager;
class Debug;
class DeviceServices;
class ImageLoader;
class IAudioManager;
class SocialServices;
class PurchaseServices;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Engine
{
  public:

    virtual ~Engine() {}

  public signals:

    /*! Signal sent at the begining of a frame. */
    Signal0<> signalFrameBegin;
    /*! Signal sent at the end of a frame. */
    Signal0<> signalFrameEnd;

  public:

    /*! Creates instance of the engine. */
    static Engine* Create();
    /*! Destroys instance of the engine. */
    static void Destroy(Engine* instance);

  public:

    /*! Initializes engine. 
     *  @param  configurationDictionary Config parameters dictionary.
     *  @return EGE_SUCCESS if engine was successfully constructed. Otherwise, one of the existing error codes.
     *  @note This method can throw.
     */
    virtual EGEResult construct(const Dictionary& configurationDictionary) = 0;

    /*! Returns configuration dictionary. */
    virtual const Dictionary& configurationDictionary() const = 0;
    /*! Returns graphics subsystem object. */
    virtual IGraphics* graphics() const = 0;
    /*! Returns event manager. */
    virtual IEventManager* eventManager() const = 0;
    /*! Returns physics manager. */
    virtual IPhysicsManager* physicsManager() const = 0;
    /*! Returns scene manager. */
    virtual ISceneManager* sceneManager() const = 0;
    /*! Returns resource manager. */
    virtual IResourceManager* resourceManager() const = 0;
    /*! Returns pointer input. */
    virtual Pointer* pointer() const = 0;
    /*! Returns overlay manager. */
    virtual IOverlayManager* overlayManager() const = 0;
    /*! Returns screen manager. */
    virtual IScreenManager* screenManager() const = 0;
    /*! Returns audio manager. */
    virtual IAudioManager* audioManager() const = 0;
    /*! Returns debug object. */
    virtual Debug* debug() const = 0;
    /*! Returns device services object. */
    virtual DeviceServices* deviceServices() const = 0;
    /*! Returns purchase services object. */
    virtual PurchaseServices* purchaseServices() const = 0;
    /*! Returns social services object. */
    virtual SocialServices* socialServices() const = 0;
    /*! Returns image loader object. */
    virtual ImageLoader* imageLoader() const = 0;

    // TAGE - Language settings object ????
    /*! Sets new language. */
    virtual void setLanguage(const String& language) = 0;
    /*! Returns current language. */
    virtual const String& language() const = 0;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_ENGINE_ENGINE_H