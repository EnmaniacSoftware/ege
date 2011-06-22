#ifndef EGE_CORE_SCREEN_H
#define EGE_CORE_SCREEN_H

/** Base interface for screen object. Screens are objects shown on top of everything else. Screens can be stacked up on each other allowing to easily create
    different view paths i.e. different menu screens which are stacked up the way user navigates. 
*/

#include <EGE.h>
#include <EGETime.h>
#include <EGEInput.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Viewport;
class Renderer;

EGE_DECLARE_SMART_CLASS(Screen, PScreen)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Screen : public Object
{
  public:

    Screen(Application* app);
    virtual ~Screen();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Updates object. */
    virtual void update(const Time& time);
    /* Pointer event. */
    virtual void pointerEvent(PPointerData data);
    /*! Renders object. */
    virtual void render(const Viewport* viewport, Renderer* renderer) = 0;
    /*! Screen is about to be shown either from complete hideout or uncovery. */
    virtual void enter() = 0;
    /*! Screen is about to be hidden. */
    virtual void leave() = 0;
    /*! Screen is about to be partially covered by another one. */
    virtual void cover() = 0;
    /*! Returns TRUE if screen has transparent regions. */
    virtual bool hasTransparency() const = 0;

  private:
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_SCREEN_H