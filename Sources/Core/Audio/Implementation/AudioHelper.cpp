#include "EGEApplication.h"
#include "EGEResources.h"
#include "Core/Audio/Interface/AudioHelper.h"
#include "Core/Audio/Sound.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PSound AudioHelper::CreateSound(Application* application, const String& name, const String& resourceGroupName)
{
  EGE_ASSERT(NULL != application);

  PSound sound;

  // find sound resource
  PResourceSound soundResource = application->resourceManager()->soundResource(name, resourceGroupName);
  if (NULL != soundResource)
  {
    // create sound instance
    sound = soundResource->createInstance();
  }

  return sound;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PSound AudioHelper::CreateSoundAndPlay(Application* application, const String& name, s32 repeatCount, const String& resourceGroupName)
{
  EGE_ASSERT(NULL != application);

  // create sound
  PSound sound = CreateSound(application, name, resourceGroupName);
  if (NULL != sound)
  {
    // play
    if (EGE_SUCCESS != sound->play(repeatCount))
    {
      // error!
      sound = NULL;
    }
  }

  return sound;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END