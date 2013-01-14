#ifndef EGE_CORE_SERVICES_PURCHASESERVICES_H
#define EGE_CORE_SERVICES_PURCHASESERVICES_H

/*! \brief Class representing in-game purchases interface. */

#include <EGE.h>
#include <EGESocial.h>
#include <EGESignal.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(PurchaseServices, PPurchaseServices)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class PurchaseServices : public Object
{
  public:
  
    PurchaseServices(Application* app);
    virtual ~PurchaseServices();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  signals:

    /*! Signal emitted when purchase is done. 
     *  @param result   Result code of transcation.
     *  @param product  Product name for which purchase was done.
     */
    Signal2<EGEResult, const String&> purchased;

  public:

    /*! Constructs object. */
    EGEResult construct();
    /*! PurchaseServices the given product. */
    EGEResult purchase(const String& product);

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(PurchaseServices)
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_SERVICES_PURCHASESERVICES_H