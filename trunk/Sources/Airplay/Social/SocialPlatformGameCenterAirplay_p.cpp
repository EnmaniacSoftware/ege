#include "Core/Social/SocialPlatform.h"
#include "Airplay/Social/SocialPlatformGameCenterAirplay_p.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(SocialPlatformPrivate)
EGE_DEFINE_DELETE_OPERATORS(SocialPlatformPrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SocialPlatformPrivate::SocialPlatformPrivate(SocialPlatform* base) : m_d(base)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SocialPlatformPrivate::~SocialPlatformPrivate()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Constructs object. */
EGEResult SocialPlatformPrivate::construct()
{
  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Starts authentication. */
EGEResult SocialPlatformPrivate::startAuthentication()
{
  // check if supported
  if ( ! s3eIOSGameCenterAvailable())
  {
    // error!
    return EGE_ERROR_NOT_SUPPORTED;
  }

  // check if authenticated already
  if (s3eIOSGameCenterGetInt(S3E_IOSGAMECENTER_LOCAL_PLAYER_IS_AUTHENTICATED))
  {
    // done
    // TAGE - should be emit ?
    return EGE_SUCCESS;
  }

  // start authentication
  return (S3E_RESULT_SUCCESS == s3eIOSGameCenterAuthenticate(AuthenticationCallback, this)) ? EGE_SUCCESS : EGE_ERROR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Authentication callback. */
void SocialPlatformPrivate::AuthenticationCallback(s3eIOSGameCenterError* error, void* userData)
{
  SocialPlatformPrivate* me = (SocialPlatformPrivate*) userData;

  EGEResult result = EGE_ERROR;

  // map error code to framework values
  switch (*error)
  {
    case S3E_IOSGAMECENTER_ERR_NONE:        result = EGE_SUCCESS; break;
    case S3E_IOSGAMECENTER_ERR_UNSUPPORTED: result = EGE_ERROR_NOT_SUPPORTED; break;
  }

  // store flag
  me->d_func()->m_authenticated = (EGE_SUCCESS == result);

  // emit
  emit me->d_func()->authenticated(result);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Loads achievements. */
EGEResult SocialPlatformPrivate::loadAchievements()
{
  // request achievement list
  return (S3E_RESULT_SUCCESS == s3eIOSGameCenterLoadAchievements(LoadAchievementsCallback)) ? EGE_SUCCESS : EGE_ERROR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Achievement list load callback. */
void SocialPlatformPrivate::LoadAchievementsCallback(s3eIOSGameCenterAchievementList* list)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Save achievements. */
EGEResult SocialPlatformPrivate::saveAchievements(const AchievementDataList& achievements)
{
  bool empty = m_achievementSaveSessions.empty();
  
  // add to session pool
  m_achievementSaveSessions.push_back(achievements);

  // process next achievement
  return (empty) ? saveNextAchievement() : EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Saves next achievement from first session. */
EGEResult SocialPlatformPrivate::saveNextAchievement()
{
  AchievementDataList& list = m_achievementSaveSessions.front();

  // check if anything to be saved from current list
  if (list.empty())
  {
    // remove list from pool
    m_achievementSaveSessions.removeAt(0);
  }

  AchievementData achievementData;

  //char name[255];
  //const char* tmp = s3eOSReadStringUTF8("Enter achievement name:");
  //strcpy(name, tmp);
  //const char* valueStr = s3eOSReadStringUTF8("Percent Complete:", S3E_OSREADSTRING_FLAG_NUMBER);
  //int value = atoi(valueStr);

  // submit achievement
  s3eResult result = s3eIOSGameCenterReportAchievement(achievementData.name.toAscii(), achievementData.percentageComplete, AchievementSaveCallback);
  return (S3E_RESULT_SUCCESS == result) ? EGE_SUCCESS : EGE_ERROR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Achievement save callback. */
void SocialPlatformPrivate::AchievementSaveCallback(s3eIOSGameCenterError* error)
{
  EGEResult result = EGE_ERROR;

  // map error code to framework values
  switch (*error)
  {
    case S3E_IOSGAMECENTER_ERR_NONE: result = EGE_SUCCESS; break;
  }

}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END