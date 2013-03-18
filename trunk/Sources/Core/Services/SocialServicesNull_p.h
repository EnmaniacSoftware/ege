#ifndef EGE_CORE_SERVICES_SOCIALSERVICES_PRIVATE_H
#define EGE_CORE_SERVICES_SOCIALSERVICES_PRIVATE_H

/*! \brief Null (mock) implementation for social platform. */

#include "EGE.h"

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
    /*! Save achievements. */
    EGEResult saveAchievements(const AchievementDataList& achievements);
    /*! Saves score. */
    EGEResult saveScore(const String& scoreTable, s32 score);
    /*! Shows scores. */
    EGEResult showScores(const String& scoreTable);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_SERVICES_SOCIALSERVICES_PRIVATE_H