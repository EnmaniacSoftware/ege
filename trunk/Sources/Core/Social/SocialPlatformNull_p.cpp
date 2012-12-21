#include "Core/Social/SocialPlatform.h"
#include "Core/Social/SocialPlatformNull_p.h"

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
EGEResult SocialPlatformPrivate::construct()
{
  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SocialPlatformPrivate::startAuthentication()
{
  return EGE_ERROR_NOT_SUPPORTED;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SocialPlatformPrivate::loadAchievements()
{
  return EGE_ERROR_NOT_SUPPORTED;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SocialPlatformPrivate::saveAchievements(const AchievementDataList& achievements)
{
  EGE_UNUSED(achievements);
  return EGE_ERROR_NOT_SUPPORTED;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SocialPlatformPrivate::saveScore(const String& scoreTable, s32 score)
{
  EGE_UNUSED(scoreTable);
  EGE_UNUSED(score);
  return EGE_ERROR_NOT_SUPPORTED;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SocialPlatformPrivate::showScores(const String& scoreTable)
{
  EGE_UNUSED(scoreTable);
  return EGE_ERROR_NOT_SUPPORTED;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END