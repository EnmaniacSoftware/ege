#ifndef EGE_CORE_SERVICES_SOCIALSERVICES_H
#define EGE_CORE_SERVICES_SOCIALSERVICES_H

/*! Interface for classes providing social functionality.  
 */

#include "EGE.h"
#include "EGEString.h"
#include "EGEList.h"
#include "EGESignal.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Data struct defining achievement information. */
struct AchievementData
{
  String name;                /*!< Achievement identifier. */
  s32 percentageComplete;     /*!< Percentage complete. */
};

typedef List<AchievementData> AchievementDataList;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class SocialServices
{
  public:
  
    SocialServices(Application* application);
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
    /*! Signal emitted when social screen is dismissed. */
    Signal0<> screenDismissed;
  
  public:

    /*! Starts authentication. */
    virtual EGEResult startAuthentication() = 0;
    /*! Loads achievements. */
    virtual EGEResult loadAchievements() = 0;
    /*! Save achievements. */
    virtual EGEResult saveAchievements(const AchievementDataList& achievements) = 0;
    /*! Saves score. */
    virtual EGEResult saveScore(const String& scoreTable, s32 score) = 0;
    /*! Shows scores. */
    virtual EGEResult showScores(const String& scoreTable) = 0;
    /*! Returns TRUE if services are authenticated and ready to be used. */
    virtual bool isAuthenticated() const = 0;

  protected:

    /*! Returns pointer to application object. */
    Application* app() const;

  private:

    /*! Pointer to application object. */
    Application* m_application;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_SERVICES_SOCIALSERVICES_H