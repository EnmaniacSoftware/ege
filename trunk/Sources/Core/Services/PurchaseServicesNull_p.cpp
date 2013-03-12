#include "Core/Services/PurchaseServices.h"
#include "Core/Services/PurchaseServicesNull_p.h"

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
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult PurchaseServicesPrivate::construct()
{
  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult PurchaseServicesPrivate::purchase(const String& product)
{
  EGE_UNUSED(product);
  return EGE_ERROR_NOT_SUPPORTED;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult PurchaseServicesPrivate::restoreAll()
{
  return EGE_ERROR_NOT_SUPPORTED;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END