#ifndef EGE_IOS_SERVICES_SOCIALSERVICES_H
#define EGE_IOS_SERVICES_SOCIALSERVICES_H

/*! iOS Game Center implementation for social platform. 
 */

#include "EGE.h"
#include "EGEList.h"
#include "EGEString.h"
#include "EGESocialServices.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class SocialServicesIOS : public SocialServices
{
  public:
    
    SocialServicesIOS(Application* application);
   ~SocialServicesIOS();

  protected:

    /*! @see SocialServices::startAuthentication. */
    EGEResult startAuthentication() override;
    /*! @see SocialServices::loadAchievements. */
    EGEResult loadAchievements() override;
    /*! @see SocialServices::saveAchievements. */
    EGEResult saveAchievements(const AchievementDataList& achievements) override;
    /*! @see SocialServices::saveScore. */
    EGEResult saveScore(const String& scoreTable, s32 score) override;
    /*! @see SocialServices::showScores. */
    EGEResult showScores(const String& scoreTable) override;
    /*! @see SocialServices::isAuthenticated. */
    bool isAuthenticated() const override;

  private:

    /*! Saves next achievement from first session. */
    EGEResult saveNextAchievement();

  private:

    /*! Game center delegate. */
    void* m_delegate;
    /*! Last authenticated user name. */
    String m_userId;
    /*! List of pending achievements to be saved. */
    AchievementDataList m_pendingAchievementSaveList;
    /*! Authenticated flag. */
    bool m_authenticated;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_IOS_SERVICES_SOCIALSERVICES_H