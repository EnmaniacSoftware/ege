#ifndef EGE_CORE_OVERLAYMANAGER_H
#define EGE_CORE_OVERLAYMANAGER_H

#include "EGE.h"
#include "EGEString.h"
#include "EGEList.h"
#include "EGETime.h"
#include "EGEOverlay.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class IRenderer;
class Viewport;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class OverlayManager : public Object
{
  public: 

    OverlayManager(Application* app);
   ~OverlayManager();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /*! Creates object. */
    EGEResult construct();
    /*! Renders all elements. */
    void render(Viewport* viewport, IRenderer* renderer);
    /*! Updates manager. */
    void update(const Time& time);
    /*! Adds overlay. 
     *  @param overlay Overlay to be added.
     *  @return  EGE_SUCCESS if operation succeeds. EGE_ERROR_ALREADY_EXISTS if overlay with such name already exists. Otherwise, EGE_ERROR.
     */
    EGEResult add(const POverlay& overlay);
    /*! Removes overlay of the given name. */
    void remove(const String& name);
    /*! Removes givem overlay. */
    void remove(const POverlay& overlay);
    /*! Removes all overlays. */
    void removeAll();
    /*! Returns overlay of the given name. */
    POverlay overlay(const String& name) const;

  private:

    typedef List<POverlay> OverlayList;

  private:

    /*! Pool of overlays. */
    OverlayList m_overlays;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_OVERLAYMANAGER_H
