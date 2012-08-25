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
/*! Constructs object. */
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
/*! Starts authentication. */
EGEResult SocialPlatform::startAuthentication()
{
  if (NULL != m_p)
  {
    return m_p->startAuthentication();
  }

  return EGE_ERROR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Loads achievements. */
EGEResult SocialPlatform::loadAchievements()
{
  if (NULL != m_p)
  {
    return m_p->loadAchievements();
  }

  return EGE_ERROR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Save achievements. */
EGEResult SocialPlatform::saveAchievements(const AchievementDataList& achievements)
{
  if (NULL != m_p)
  {
    return m_p->saveAchievements(achievements);
  }

  return EGE_ERROR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Save score. */
EGEResult SocialPlatform::saveScore(const String& scoreTable, s32 score)
{
  if (NULL != m_p)
  {
    return m_p->saveScore(scoreTable, score);
  }

  return EGE_ERROR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END