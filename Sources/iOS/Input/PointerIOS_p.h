#ifndef EGE_IOS_INPUT_POINTER_PRIVATE_H
#define EGE_IOS_INPUT_POINTER_PRIVATE_H

#include "EGE.h"
#include "EGEEvent.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Pointer;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class PointerPrivate : public IEventListener
{
  public:

    PointerPrivate(Pointer* base);
   ~PointerPrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    EGE_DECLARE_PUBLIC_IMPLEMENTATION(Pointer)

  private:
  
    /*! @see IEventListener::onEventRecieved. */
    void onEventRecieved(PEvent event) override;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_IOS_INPUT_POINTER_PRIVATE_H