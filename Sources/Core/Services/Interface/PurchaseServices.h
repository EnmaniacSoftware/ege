#ifndef EGE_CORE_SERVICES_PURCHASESERVICES_H
#define EGE_CORE_SERVICES_PURCHASESERVICES_H

/*! Interface for classes providing in-game purchases functionality. 
 */

#include "EGE.h"
#include "EGESocialServices.h"
#include "EGESignal.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class PurchaseServices
{
  public:
  
    PurchaseServices(Application* application);
    virtual ~PurchaseServices();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public signals:

    /*! Signal emitted when purchase is done. 
     *  @param result   Result code of transcation.
     *  @param product  Product name for which purchase was done.
     */
    Signal2<EGEResult, const String&> purchased;
    /*! Signal emitted when purchase restore is done. 
     *  @param result   Result code of transcation.
     */
    Signal1<EGEResult> restored;

  public:

    /*! Purchases the given product. 
     *  @param  product Product name to purchase.
     *  @return EGE_SUCCESS on success.
     */
    virtual EGEResult purchase(const String& product) = 0;
    /*! Restores all purchases made for so far. */
    virtual EGEResult restoreAll() = 0;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_SERVICES_PURCHASESERVICES_H