#ifndef EGE_CORE_PURCHASES_PRIVATE_H
#define EGE_CORE_PURCHASES_PRIVATE_H

#include <EGE.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Purchases;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! \brief Null (mock) implementation for purchases. */
class PurchasesPrivate
{
  public:
    
    PurchasesPrivate(Purchases* base);
   ~PurchasesPrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    EGE_DECLARE_PUBLIC_IMPLEMENTATION(Purchases)

  public:

    /* Constructs object. */
    EGEResult construct();
    /* Purchases the given product. */
    EGEResult purchase(const String& product);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_PURCHASES_PRIVATE_H