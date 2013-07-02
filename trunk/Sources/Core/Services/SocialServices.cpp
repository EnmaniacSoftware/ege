#include "Core/Services/SocialServices.h"
#include "Core/Services/SocialServicesNull_p.h"
#include "EGEDebug.h"

#if EGE_PLATFORM_AIRPLAY
  #include "Airplay/Services/SocialServicesGameCenterAirplay_p.h"
#elif EGE_PLATFORM_IOS
  #include "iOS/Services/SocialServicesGameCenterIOS_p.h"
#endif // EGE_PLATFORM_AIRPLAY

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(SocialServices)
EGE_DEFINE_DELETE_OPERATORS(SocialServices)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SocialServices::SocialServices(Application* app) : Object(app),
                                                   m_p(NULL),
                                                   m_authenticated(false)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SocialServices::~SocialServices()
{
  EGE_DELETE(m_p);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SocialServices::construct()
{
  EGEResult result = EGE_SUCCESS;

  // create private
  m_p = ege_new SocialServicesPrivate(this);
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
EGEResult SocialServices::startAuthentication()
{
  return p_func()->startAuthentication();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SocialServices::loadAchievements()
{
  return p_func()->loadAchievements();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SocialServices::saveAchievements(const AchievementDataList& achievements)
{
  return p_func()->saveAchievements(achievements);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SocialServices::saveScore(const String& scoreTable, s32 score)
{
  return p_func()->saveScore(scoreTable, score);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SocialServices::showScores(const String& scoreTable)
{
  return p_func()->showScores(scoreTable);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool SocialServices::isAuthenticated() const
{
  return m_authenticated;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END