#ifndef EGE_IOS_SERVICES_SOCIALSERVICES_PRIVATE_H
#define EGE_IOS_SERVICES_SOCIALSERVICES_PRIVATE_H

/*! \brief Game Center implementation for social platform. */

#if EGE_SOCIAL_SERVICES_GAMECENTER

#include "EGE.h"
#include "EGEList.h"
#include "EGEString.h"
#include "EGESocialServices.h"

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

    /*! Saves next achievement from first session. */
    EGEResult saveNextAchievement();

  private:

    /*! Game center delegate. */
    void* m_delegate;
    /*! Game center controller. */
    void* m_gameCenterController;
    /*! Last authenticated user name. */
    String m_userId;
    /*! List of pending achievements to be saved. */
    AchievementDataList m_pendingAchievementSaveList;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_SOCIAL_SERVICES_GAMECENTER

#endif // EGE_IOS_SERVICES_SOCIALSERVICES_PRIVATE_H