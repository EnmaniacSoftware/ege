#ifndef EGE_SOCIALSERVICES_H
#define EGE_SOCIALSERVICES_H

#include <EGEString.h>
#include <EGEList.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
struct AchievementData
{
  String name;                /*!< Achievement identifier. */
  s32 percentageComplete;     /*!< Percentage complete. */
};

typedef List<AchievementData> AchievementDataList;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "Core/Services/SocialServices.h"
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EGE_SOCIALSERVICES_H