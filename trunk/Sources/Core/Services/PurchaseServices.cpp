#include "Core/Services/PurchaseServices.h"
#include "Core/Services/PurchaseServicesNull_p.h"
#include "EGEDebug.h"

#if EGE_PLATFORM_AIRPLAY
  #include "Airplay/Services/PurchaseServicesAppStoreAirplay_p.h"
#elif EGE_PLATFORM_IOS
  #include "iOS/Services/PurchaseServicesAppStoreIOS_p.h"
#endif // EGE_PLATFORM_AIRPLAY

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(PurchaseServices)
EGE_DEFINE_DELETE_OPERATORS(PurchaseServices)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PurchaseServices::PurchaseServices(Application* app) : Object(app),
                                                       m_p(NULL)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PurchaseServices::~PurchaseServices()
{
  EGE_DELETE(m_p);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult PurchaseServices::construct()
{
  EGEResult result = EGE_SUCCESS;

  // create private
  m_p = ege_new PurchaseServicesPrivate(this);
  if (NULL == m_p)
  {
    // error!
    return EGE_ERROR_NO_MEMORY;
  }

  // construct
  if (EGE_SUCCESS != (result = m_p->construct()))
  {
    // error!
    return result;
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult PurchaseServices::purchase(const String& product)
{
  if (NULL != m_p)
  {
    return m_p->purchase(product);
  }

  return EGE_ERROR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult PurchaseServices::restoreAll()
{
  if (NULL != m_p)
  {
    return m_p->restoreAll();
  }

  return EGE_ERROR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END