#include "Core/Social/SocialPlatform.h"
#include <EGEDebug.h>

#ifdef EGE_SOCIAL_PLATFORM_GAMECENTER
  #ifdef EGE_PLATFORM_AIRPLAY
    #include "Airplay/Social/SocialPlatformGameCenterAirplay_p.h"
  #else
    #error NO GAME CENTER IMPLEMENTATION AVAILABLE ?
  #endif // EGE_PLATFORM_AIRPLAY
#else
  #include "Core/Social/SocialPlatformNull_p.h"
#endif // EGE_SOCIAL_PLATFORM_GAMECENTER

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(SocialPlatform)
EGE_DEFINE_DELETE_OPERATORS(SocialPlatform)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SocialPlatform::SocialPlatform(Application* app) : Object(app),
                                                   m_p(NULL),
                                                   m_authenticated(false)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SocialPlatform::~SocialPlatform()
{
  EGE_DELETE(m_p);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SocialPlatform::construct()
{
  EGEResult result = EGE_SUCCESS;

  // create private
  m_p = ege_new SocialPlatformPrivate(this);
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
EGEResult SocialPlatform::startAuthentication()
{
  return p_func()->startAuthentication();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SocialPlatform::loadAchievements()
{
  return p_func()->loadAchievements();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SocialPlatform::saveAchievements(const AchievementDataList& achievements)
{
  return p_func()->saveAchievements(achievements);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SocialPlatform::saveScore(const String& scoreTable, s32 score)
{
  return p_func()->saveScore(scoreTable, score);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SocialPlatform::showScores(const String& scoreTable)
{
  return p_func()->showScores(scoreTable);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END