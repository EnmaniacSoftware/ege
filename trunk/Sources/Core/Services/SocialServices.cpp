#include "Core/Services/SocialServices.h"
#include "EGEDebug.h"

#ifdef EGE_SOCIAL_PLATFORM_GAMECENTER
  #ifdef EGE_PLATFORM_AIRPLAY
    #include "Airplay/Services/SocialServicesGameCenterAirplay_p.h"
  #else
    #error NO GAME CENTER IMPLEMENTATION AVAILABLE ?
  #endif // EGE_PLATFORM_AIRPLAY
#else
  #include "Core/Services/SocialServicesNull_p.h"
#endif // EGE_SOCIAL_PLATFORM_GAMECENTER

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

EGE_NAMESPACE_END