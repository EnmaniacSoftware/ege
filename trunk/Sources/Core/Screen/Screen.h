#ifndef EGE_CORE_SCREEN_H
#define EGE_CORE_SCREEN_H

#include <EGE.h>
#include <EGETime.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

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
    /*! Screen is about to be shown. */
    virtual void enter() = 0;
    /*! Screen is about to be hidden. */
    virtual void leave() = 0;

  private:
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_SCREEN_H