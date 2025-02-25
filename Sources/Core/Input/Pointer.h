#ifndef EGE_CORE_POINTER_H
#define EGE_CORE_POINTER_H

#include "EGE.h"
#include "EGESignal.h"
#include "EGEInput.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(PointerData, PPointerData)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Pointer : public Object
{
  public:

    Pointer(Application* app);
   ~Pointer();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public signals:

    /*! Signal emited on any pointer event. 
     *  @param pointerData Data describing pointer event.
     */
    Signal1<PPointerData> eventSignal; 

  public:

    /*! Creates object. */
    EGEResult construct();

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(Pointer)
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_POINTER_H