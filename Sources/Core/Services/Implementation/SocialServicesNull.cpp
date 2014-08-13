#include "Core/Services/Interface/SocialServicesNull.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SocialServicesNull::SocialServicesNull(Engine& engine) : SocialServices(engine)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SocialServicesNull::~SocialServicesNull()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SocialServicesNull::startAuthentication()
{
  return EGE_ERROR_NOT_SUPPORTED;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SocialServicesNull::loadAchievements()
{
  return EGE_ERROR_NOT_SUPPORTED;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SocialServicesNull::saveAchievements(const AchievementDataList& achievements)
{
  EGE_UNUSED(achievements);
  return EGE_ERROR_NOT_SUPPORTED;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SocialServicesNull::saveScore(const String& scoreTable, s32 score)
{
  EGE_UNUSED(scoreTable);
  EGE_UNUSED(score);
  return EGE_ERROR_NOT_SUPPORTED;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult SocialServicesNull::showScores(const String& scoreTable)
{
  EGE_UNUSED(scoreTable);
  return EGE_ERROR_NOT_SUPPORTED;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool SocialServicesNull::isAuthenticated() const
{
  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END