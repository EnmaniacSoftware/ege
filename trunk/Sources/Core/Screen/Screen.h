#ifndef EGE_CORE_SCREEN_H
#define EGE_CORE_SCREEN_H

/** Base interface for screen object. Screens are objects shown on top of everything else. Screens can be stacked up on each other allowing to easily create
    different view paths i.e. different menu screens which are stacked up the way user navigates. 
*/

#include <EGE.h>
#include <EGETime.h>
#include <EGEInput.h>
#include <EGEString.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Viewport;
class Renderer;

EGE_DECLARE_SMART_CLASS(Screen, PScreen)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Screen : public Object
{
  public:

    Screen(Application* app, const String& name = "");
    virtual ~Screen();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Updates object. */
    virtual void update(const Time& time);
    /* Pointer event. */
    virtual void pointerEvent(PPointerData data);
    /* Renders object. */
    virtual void render(const Viewport* viewport, Renderer* renderer);
    /* Screen is about to be shown either from complete hideout or uncovery. */
    virtual void enter();
    /* Screen is about to be hidden. */
    virtual void leave();
    /* Screen is about to be partially covered by another one. */
    virtual void cover();
    /*! Returns TRUE if screen is partially covered by another one. */
    inline bool isCovered() const { return m_covered; }
    /*! Returns TRUE if screen has transparent regions. */
    virtual bool hasTransparency() const = 0;
    /* Enables/disables screen. */
    void setEnable(bool enable);
    /*! Returns TRUE if screen is enabled. */
    inline bool isEnabled() const { return !m_disabled; }
    /*! Returns name. */
    const String& name() const { return m_name; }

  private:

    /*! Name. */
    String m_name;
    /*! Disabled flag. */
    bool m_disabled;
    /*! Cover flag. */
    bool m_covered;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_SCREEN_H