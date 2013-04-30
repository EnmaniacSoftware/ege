#ifndef EGE_CORE_SERVICES_SOCIALSERVICES_H
#define EGE_CORE_SERVICES_SOCIALSERVICES_H

/*! \brief Class representing social platform. */

#include "EGE.h"
#include "EGESocialServices.h"
#include "EGESignal.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(SocialServices, PSocialServices)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class SocialServices : public Object
{
  public:
  
    SocialServices(Application* app);
    virtual ~SocialServices();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public signals:

    /*! Signal emitted when authentication is done. 
     *  @param result Result code of authentication.
     */
    Signal1<EGEResult> authenticated;
    /*! Signal emitted when achievement list retrieval is done.
     *  @param result Result code.
     *  @param list   List of achievements.
     */
    Signal2<EGEResult, const AchievementDataList&> achievementsLoaded;
    /*! Signal emitted when submission of achievements is done. 
     *  @param result Result code of submission.
     */
    Signal1<EGEResult> achievementsSaved;
    /*! Signal emitted when submission of score is done. 
     *  @param result Result code of submission.
     */
    Signal1<EGEResult> scoreSaved;

  public:

    /*! Constructs object. */
    EGEResult construct();
    /*! Starts authentication. */
    EGEResult startAuthentication();
    /*! Loads achievements. */
    EGEResult loadAchievements();
    /*! Save achievements. */
    EGEResult saveAchievements(const AchievementDataList& achievements);
    /*! Saves score. */
    EGEResult saveScore(const String& scoreTable, s32 score);
    /*! Shows scores. */
    EGEResult showScores(const String& scoreTable);

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(SocialServices)

    /*! Flag indicating whether logged into platform. */
    bool m_authenticated;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_SERVICES_SOCIALSERVICES_H