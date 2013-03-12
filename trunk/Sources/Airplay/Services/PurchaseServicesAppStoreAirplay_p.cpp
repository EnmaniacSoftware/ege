#include "Core/Services/PurchaseServices.h"
#include "Airplay/Services/PurchaseServicesAppStoreAirplay_p.h"
#include <EGEMath.h>
#include <EGEMemory.h>
#include <EGEDebug.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(PurchaseServicesPrivate)
EGE_DEFINE_DELETE_OPERATORS(PurchaseServicesPrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PurchaseServicesPrivate::PurchaseServicesPrivate(PurchaseServices* base) : m_d(base)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PurchaseServicesPrivate::~PurchaseServicesPrivate()
{
  s3eIOSAppStoreBillingStop();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult PurchaseServicesPrivate::construct()
{
  // check if supported
  if ( ! s3eIOSAppStoreBillingAvailable())
  {
    // error!
    return EGE_ERROR_NOT_SUPPORTED;
  }

  // register callbacks
  return (S3E_RESULT_SUCCESS == s3eIOSAppStoreBillingStart(ProductInfoCallback, TransactionUpdateCallback, this)) ? EGE_SUCCESS : EGE_ERROR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PurchaseServicesPrivate::ProductInfoCallback(s3eProductInformation* productInfo, void* userData)
{
  PurchaseServicesPrivate* me = static_cast<PurchaseServicesPrivate*>(userData);

  // process accroding to status
  switch (productInfo->m_ProductStoreStatus)
  {
    case S3E_PRODUCT_STORE_STATUS_RESTORE_COMPLETED:

      // emit
      emit me->d_func()->restored(EGE_SUCCESS);
      break;

    case S3E_PRODUCT_STORE_STATUS_RESTORE_FAILED:

      // emit
      emit me->d_func()->restored(EGE_ERROR);
      break;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PurchaseServicesPrivate::TransactionUpdateCallback(s3ePaymentTransaction* transaction, void* userData)
{
  PurchaseServicesPrivate* me = static_cast<PurchaseServicesPrivate*>(userData);

  EGEResult result = EGE_ERROR;

  egeDebug() << "TRANSACTION UPDATE" << transaction->m_TransactionStatus;

  // process accroding to status
  switch (transaction->m_TransactionStatus)
  {
    case S3E_PAYMENT_STATUS_PENDING:

      result = EGE_WAIT;
      break;

    case S3E_PAYMENT_STATUS_PURCHASED:
    case S3E_PAYMENT_STATUS_RESTORED:

      result = EGE_SUCCESS;
      break;

    case S3E_PAYMENT_STATUS_FAILED_CLIENT_INVALID:

      result = EGE_ERROR;
      break;

    case S3E_PAYMENT_STATUS_FAILED_PAYMENT_CANCELLED:

      result = EGE_ERROR;
      break;

    case S3E_PAYMENT_STATUS_FAILED_PAYMENT_INVALID:

      result = EGE_ERROR;
      break;

    case S3E_PAYMENT_STATUS_FAILED_PAYMENT_NOT_ALLOWED:

      result = EGE_ERROR;
      break;

    default:

      result = EGE_ERROR;
      break;
  }

  // emit if done
  if (EGE_WAIT != result)
  {
    // emit
    emit me->d_func()->purchased(result, transaction->m_Request->m_ProductID);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult PurchaseServicesPrivate::purchase(const String& product)
{
  s3ePaymentRequest paymentRequest;
  
  EGE_MEMCPY(paymentRequest.m_ProductID, product.toAscii(), Math::Min(product.length() + 1, sizeof (paymentRequest.m_ProductID)));
  paymentRequest.m_Quantity  = 1;

  return (S3E_RESULT_SUCCESS == s3eIOSAppStoreBillingRequestPayment(&paymentRequest)) ?  EGE_SUCCESS : EGE_ERROR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult PurchaseServicesPrivate::restoreAll()
{
  return (S3E_RESULT_SUCCESS == s3eIOSAppStoreBillingRestoreCompletedTransactions()) ?  EGE_SUCCESS : EGE_ERROR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END