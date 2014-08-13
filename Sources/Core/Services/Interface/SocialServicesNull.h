#ifndef EGE_CORE_SERVICES_SOCIALSERVICESNULL_H
#define EGE_CORE_SERVICES_SOCIALSERVICESNULL_H

/*! Null implementation for social services. This is mainly to be used as quick ready-to-use code with no real implementation.
 *  This should not to be used but its own. Platform specific implementations should rather derive from it.
 */

#include "EGE.h"
#include "Core/Services/Interface/SocialServices.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class SocialServicesNull : public SocialServices
{
  public:
    
    SocialServicesNull(Engine& engine);
   ~SocialServicesNull();

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
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_SERVICES_SOCIALSERVICESNULL_H