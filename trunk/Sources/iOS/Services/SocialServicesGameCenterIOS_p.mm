#include "Core/Services/SocialServices.h"
#include "iOS/Services/SocialServicesGameCenterIOS_p.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static SocialServicesPrivate* l_instance = NULL;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(SocialServicesPrivate)
EGE_DEFINE_DELETE_OPERATORS(SocialServicesPrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SocialServicesPrivate::SocialServicesPrivate(SocialServices* base) : m_d(base)
{
  // NOTE: this is due to Airplay API limitation where it is not possible to add user data to callbacks
  //       Do not create more that one instance of this class
  l_instance = this;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SocialServicesPrivate::~SocialServicesPrivate()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SocialServicesPrivate::construct()
{
  // check if supported
 /* if ( ! s3eIOSGameCenterAvailable())
  {
    // error!
    return EGE_ERROR_NOT_SUPPORTED;
  }*/

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SocialServicesPrivate::startAuthentication()
{
  // check if supported
  /*if ( ! s3eIOSGameCenterAvailable())
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
  }*/

  // start authentication
  return EGE_ERROR;// (S3E_RESULT_SUCCESS == s3eIOSGameCenterAuthenticate(AuthenticationCallback, NULL)) ? EGE_SUCCESS : EGE_ERROR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void SocialServicesPrivate::AuthenticationCallback(s3eIOSGameCenterError* error, void* userData)
{
  EGEResult result = EGE_ERROR;

  // map error code to framework values
 /* switch (*error)
  {
    case S3E_IOSGAMECENTER_ERR_NONE:        result = EGE_SUCCESS; break;
    case S3E_IOSGAMECENTER_ERR_UNSUPPORTED: result = EGE_ERROR_NOT_SUPPORTED; break;
  }

  egeWarning() << EGE_FUNC_INFO << *error;

  // store flag
  l_instance->d_func()->m_authenticated = (EGE_SUCCESS == result);

  // emit
  emit l_instance->d_func()->authenticated(result);*/
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SocialServicesPrivate::loadAchievements()
{
  // check if supported
 /* if ( ! s3eIOSGameCenterAvailable())
  {
    // error!
    return EGE_ERROR_NOT_SUPPORTED;
  }
  */
  // request achievement list
  return EGE_ERROR;//(S3E_RESULT_SUCCESS == s3eIOSGameCenterLoadAchievements(LoadAchievementsCallback)) ? EGE_SUCCESS : EGE_ERROR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*void SocialServicesPrivate::LoadAchievementsCallback(s3eIOSGameCenterAchievementList* list)
{
  EGEResult result = EGE_ERROR;

  AchievementDataList achievementsList;

  // map error code to framework values
  switch (list->m_Error)
  {
    case S3E_IOSGAMECENTER_ERR_NONE: result = EGE_SUCCESS; break;
  }

  // process achievements if successful
  if (EGE_SUCCESS == result)
  {
    for (int i = 0; i < list->m_AchievementCount; ++i)
    {
      AchievementData achievementData;
      achievementData.name               = list->m_Achievements[i].m_Identifier;
      achievementData.percentageComplete = list->m_Achievements[i].m_PercentComplete;

      achievementsList.push_back(achievementData);
    }
  }

  // emit
  emit l_instance->d_func()->achievementsLoaded(result, achievementsList);
}*/
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SocialServicesPrivate::saveAchievements(const AchievementDataList& achievements)
{
  // check if supported
 /* if ( ! s3eIOSGameCenterAvailable())
  {
    // error!
    return EGE_ERROR_NOT_SUPPORTED;
  }*/

  egeDebug() << "Adding" << achievements.size() << "achievements for submission";

  // append
  m_pendingAchievementSaveList << achievements;

  // process next achievement
  return ((m_pendingAchievementSaveList.size() == achievements.size()) && ! achievements.empty()) ? saveNextAchievement() : EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SocialServicesPrivate::saveNextAchievement()
{
  AchievementData achievementData = m_pendingAchievementSaveList.front();
  m_pendingAchievementSaveList.pop_front();

  egeDebug() << "Submitting achievement" << achievementData.name << "completion" << achievementData.percentageComplete;

  // submit achievement
 // s3eResult result = s3eIOSGameCenterReportAchievement(achievementData.name.toAscii(), achievementData.percentageComplete, AchievementSaveCallback);
  return EGE_SUCCESS;//(S3E_RESULT_SUCCESS == result) ? EGE_SUCCESS : EGE_ERROR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*void SocialServicesPrivate::AchievementSaveCallback(s3eIOSGameCenterError* error)
{
  EGEResult result = EGE_ERROR;

  // map error code to framework values
  switch (*error)
  {
    case S3E_IOSGAMECENTER_ERR_NONE: result = EGE_SUCCESS; break;
  }

  // check if error
  if (EGE_SUCCESS != result)
  {
    // clean up pending queue
    l_instance->m_pendingAchievementSaveList.clear();

    // emit
    emit l_instance->d_func()->achievementsSaved(result);
  }
  else
  {
    // check if any achievement still in queue
    if ( ! l_instance->m_pendingAchievementSaveList.empty())
    {
      // proceed
      if (EGE_SUCCESS != (result = l_instance->saveNextAchievement()))
      {
        // clean up pending queue
        l_instance->m_pendingAchievementSaveList.clear();

        // emit
        emit l_instance->d_func()->achievementsSaved(result);
      }
    }
    else
    {
      // done, emit
      emit l_instance->d_func()->achievementsSaved(result);
    }
  }
}*/
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SocialServicesPrivate::saveScore(const String& scoreTable, s32 score)
{
  // check if supported
/*  if ( ! s3eIOSGameCenterAvailable())
  {
    // error!
    return EGE_ERROR_NOT_SUPPORTED;
  }*/

  // submit achievement
  //s3eResult result = s3eIOSGameCenterReportScore(score, scoreTable.toAscii(), ScoreSaveCallback);
  return EGE_ERROR;//(S3E_RESULT_SUCCESS == result) ? EGE_SUCCESS : EGE_ERROR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*void SocialServicesPrivate::ScoreSaveCallback(s3eIOSGameCenterError* error)
{
  EGEResult result = EGE_ERROR;

  // map error code to framework values
  switch (*error)
  {
    case S3E_IOSGAMECENTER_ERR_NONE: result = EGE_SUCCESS; break;
  }

  // emit
  emit l_instance->d_func()->scoreSaved(result);
}*/
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SocialServicesPrivate::showScores(const String& scoreTable)
{
  //s3eResult result = s3eIOSGameCenterLeaderboardShowGUI(scoreTable.toAscii(), S3E_IOSGAMECENTER_PLAYER_SCOPE_ALL_TIME);
  return EGE_ERROR;//(S3E_RESULT_SUCCESS == result) ? EGE_SUCCESS : EGE_ERROR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END