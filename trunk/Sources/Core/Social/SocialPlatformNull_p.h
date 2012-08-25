#ifndef EGE_CORE_SOCIALPLATFORM_PRIVATE_H
#define EGE_CORE_SOCIALPLATFORM_PRIVATE_H

#include <EGE.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class SocialPlatform;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! \brief Null (mock) implementation for social platform. */
class SocialPlatformPrivate
{
  public:
    
    SocialPlatformPrivate(SocialPlatform* base);
   ~SocialPlatformPrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    EGE_DECLARE_PUBLIC_IMPLEMENTATION(SocialPlatform)

  public:

    /* Constructs object. */
    EGEResult construct();
    /* Starts authentication. */
    EGEResult startAuthentication();
    /* Loads achievements. */
    EGEResult loadAchievements();
    /* Save achievements. */
    EGEResult saveAchievements(const AchievementDataList& achievements);
    /* Saves score. */
    EGEResult saveScore(const String& scoreTable, s32 score);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_SOCIALPLATFORM_PRIVATE_H