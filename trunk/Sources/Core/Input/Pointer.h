#ifndef EGE_CORE_POINTER_H
#define EGE_CORE_POINTER_H

#include "EGE.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Pointer : public Object
{
  public:

    Pointer(Application* app);
   ~Pointer();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Returns TRUE if object is valid. */
    bool isValid() const;

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(Pointer)
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_POINTER_H