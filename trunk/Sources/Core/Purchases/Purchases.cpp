#include "Core/Purchases/Purchases.h"
#include <EGEDebug.h>

#ifdef EGE_PURCHASES_APPSTORE

#ifdef EGE_PLATFORM_AIRPLAY
#include "Airplay/Purchases/PurchasesAppStoreAirplay_p.h"
#else
#error NO APP STORE IMPLEMENTATION AVAILABLE ?
#endif // EGE_PLATFORM_AIRPLAY

#else
#include "Core/Purchases/PurchasesNull_p.h"
#endif // EGE_SOCIAL_PLATFORM_GAMECENTER

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(Purchases)
EGE_DEFINE_DELETE_OPERATORS(Purchases)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Purchases::Purchases(Application* app) : Object(app),
                                         m_p(NULL)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Purchases::~Purchases()
{
  EGE_DELETE(m_p);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Constructs object. */
EGEResult Purchases::construct()
{
  EGEResult result = EGE_SUCCESS;

  // create private
  m_p = ege_new PurchasesPrivate(this);
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
/*! Purchases the given product. */
EGEResult Purchases::purchase(const String& product)
{
  if (NULL != m_p)
  {
    return m_p->purchase(product);
  }

  return EGE_ERROR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END