#ifndef EGE_CORE_SCREEN_H
#define EGE_CORE_SCREEN_H

/** Base interface for screen object. Screens are objects shown on top of everything else. Screens can be stacked up on each other allowing to easily create
    different view paths i.e. different menu screens which are stacked up the way user navigates. 
*/

#include "EGE.h"
#include "EGEInput.h"
#include "EGEString.h"
#include "EGETime.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Viewport;
class IRenderer;
EGE_DECLARE_SMART_CLASS(Screen, PScreen)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Screen : public Object
{
  public:

    Screen(Engine& engine, const String& name = "");
    virtual ~Screen();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! Constructs object. */
    virtual EGEResult construct();
    /*! Updates object. 
     *  @param time Time increment for which update should be performed.
     *  @return  Returns TRUE to consume the event. When consumed no underlying screens will be updated. 
     */
    virtual bool update(const Time& time);
    /*! Renders object. */
    virtual void render(const Viewport* viewport, IRenderer* renderer);
    /*! Screen is about to be shown for the first time. */
    virtual void enter();
    /*! Screen is about to be hidden. */
    virtual void leave();
    /*! Screen is about to be covered by another one. */
    virtual void cover();
    /*! Screen is about to be uncovered. */
    virtual void uncover();
    /*! Returns TRUE if screen has transparent regions. */
    virtual bool hasTransparency() const = 0;
    /*! Enables/disables screen. */
    void setEnable(bool enable);
    /*! Returns TRUE if screen is enabled. */
    bool isEnabled() const;
    /*! Returns name. */
    const String& name() const;

    /*! Called when pointer event happens.
     *  @param  event Pointer event.
     */
    virtual void onPointerEvent(const PointerEvent& event);

  protected:

    /*! Returns engine object. */
    Engine& engine() const;

  private:

    /*! Reference to engine. */
    Engine& m_engine;
    /*! Name. */
    String m_name;
    /*! Disabled flag. */
    bool m_disabled;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_SCREEN_H