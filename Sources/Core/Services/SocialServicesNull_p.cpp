#if EGE_SOCIAL_SERVICES_NULL

#include "Core/Services/SocialServices.h"
#include "Core/Services/SocialServicesNull_p.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(SocialServicesPrivate)
EGE_DEFINE_DELETE_OPERATORS(SocialServicesPrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SocialServicesPrivate::SocialServicesPrivate(SocialServices* base) : m_d(base)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SocialServicesPrivate::~SocialServicesPrivate()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SocialServicesPrivate::construct()
{
  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SocialServicesPrivate::startAuthentication()
{
  return EGE_ERROR_NOT_SUPPORTED;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SocialServicesPrivate::loadAchievements()
{
  return EGE_ERROR_NOT_SUPPORTED;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SocialServicesPrivate::saveAchievements(const AchievementDataList& achievements)
{
  EGE_UNUSED(achievements);
  return EGE_ERROR_NOT_SUPPORTED;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SocialServicesPrivate::saveScore(const String& scoreTable, s32 score)
{
  EGE_UNUSED(scoreTable);
  EGE_UNUSED(score);
  return EGE_ERROR_NOT_SUPPORTED;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SocialServicesPrivate::showScores(const String& scoreTable)
{
  EGE_UNUSED(scoreTable);
  return EGE_ERROR_NOT_SUPPORTED;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_SOCIAL_SERVICES_NULL