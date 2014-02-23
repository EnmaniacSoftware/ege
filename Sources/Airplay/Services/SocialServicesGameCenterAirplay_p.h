#ifndef EGE_AIRPLAY_SERVICES_SOCIALSERVICES_PRIVATE_H
#define EGE_AIRPLAY_SERVICES_SOCIALSERVICES_PRIVATE_H

/*! \brief Game Center implementation for social platform. */

#include <EGE.h>
#include <EGEList.h>
#include <s3eIOSGameCenter.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class SocialServices;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class SocialServicesPrivate
{
  public:
    
    SocialServicesPrivate(SocialServices* base);
   ~SocialServicesPrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    EGE_DECLARE_PUBLIC_IMPLEMENTATION(SocialServices)

  public:

    /*! Constructs object. */
    EGEResult construct();
    /*! Starts authentication. */
    EGEResult startAuthentication();
    /*! Loads achievements. */
    EGEResult loadAchievements();
    /*! Saves achievements. */
    EGEResult saveAchievements(const AchievementDataList& achievements);
    /*! Saves score. */
    EGEResult saveScore(const String& scoreTable, s32 score);
    /*! Shows scores. */
    EGEResult showScores(const String& scoreTable);

  private:

    /*! Authentication callback. */
    static void AuthenticationCallback(s3eIOSGameCenterError* error, void* userData);
    /*! Achievement list load callback. */
    static void LoadAchievementsCallback(s3eIOSGameCenterAchievementList* list);
    /*! Achievement save callback. */
    static void AchievementSaveCallback(s3eIOSGameCenterError* error);
    /*! Score save callback. */
    static void ScoreSaveCallback(s3eIOSGameCenterError* error);

  private:

    /*! Saves next achievement from first session. */
    EGEResult saveNextAchievement();

  private:

    /*! List of pending achievements to be saved. */
    AchievementDataList m_pendingAchievementSaveList;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_AIRPLAY_SERVICES_SOCIALSERVICES_PRIVATE_H