#ifndef EGE_CORE_OVERLAY_OVERLAYMANAGER_INTERFACE_H
#define EGE_CORE_OVERLAY_OVERLAYMANAGER_INTERFACE_H

#include "EGE.h"
#include "EGEString.h"
#include "EGEOverlay.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class IRenderer;
class Viewport;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class IOverlayManager
{
  public: 

    virtual ~IOverlayManager() {}

    /*! Renders all elements. TAGE - probably should not be a part of this interface as it is external interface. 
     */
    virtual void render(Viewport* viewport, IRenderer* renderer) = 0;
    
    /*! Adds overlay. 
     *  @param overlay Overlay to be added.
     *  @return  EGE_SUCCESS if operation succeeds. EGE_ERROR_ALREADY_EXISTS if overlay with such name already exists. Otherwise, EGE_ERROR.
     */
    virtual EGEResult add(const POverlay& overlay) = 0;
    /*! Removes overlay. 
     *  @param  name  Name of the overlay to remove.
     */
    virtual void remove(const String& name) = 0;
    /*! Removes overlay. 
     *  @param  overlay Overlay to remove.
     */
    virtual void remove(const POverlay& overlay) = 0;
    /*! Removes all overlays. */
    virtual void removeAll() = 0;
    /*! Returns overlay.
     *  @param  name  Name of the overlay to find.
     *  @return Found overlay. NULL if overlay could not be found.
     */
    virtual POverlay overlay(const String& name) const = 0;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_OVERLAY_OVERLAYMANAGER_INTERFACE_H
