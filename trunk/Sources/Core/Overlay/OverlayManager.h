#ifndef EGE_CORE_OVERLAYMANAGER_H
#define EGE_CORE_OVERLAYMANAGER_H

#include <EGE.h>
#include <EGEString.h>
#include <EGEList.h>
#include <EGETime.h>
#include <EGEOverlay.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Renderer;
class Viewport;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class OverlayManager : public Object
{
  public: 

    OverlayManager(Application* app);
   ~OverlayManager();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Renders all elements. */
    void render(Viewport* viewport, Renderer* renderer);
    /* Updates manager. */
    void update(const Time& time);
    /* Adds text overlay of the given name. */
    PTextOverlay addTextOverlay(const EGEString& name);
    /* Adds image overlay of the given name. */
    PImageOverlay addImageOverlay(const EGEString& name);
    /* Removes overlay of the given name. */
    void removeOverlay(const EGEString& name);
    /* Removes all overlays. */
    void removeAllOverlays();
    /* Returns overlay of the given name. */
    POverlay overlay(const EGEString& name) const;

  private:

    /*! Pool of overlays. */
    EGEList<POverlay> m_overlays;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_OVERLAYMANAGER_H
