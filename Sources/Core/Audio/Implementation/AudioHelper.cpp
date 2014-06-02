#include "EGEApplication.h"
#include "EGEResources.h"
#include "Core/Audio/Interface/AudioHelper.h"
#include "Core/Audio/Sound.h"
#include "Core/Audio/SoundEffectFadeIn.h"
#include "Core/Audio/SoundEffectFadeOut.h"

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
PSoundEffect AudioHelper::FadeIn(PSound& sound, Time& duration)
{
  EGE_ASSERT(0 < duration.microseconds());

  // create effect
  PSoundEffect effect = ege_new SoundEffectFadeIn(duration);
  if (NULL != effect)
  {
    // add effect to sound
    if ( ! sound->addEffect(effect))
    {
      // error, clean up
      effect = NULL;
    }
  }

  return effect;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PSoundEffect AudioHelper::FadeOut(PSound& sound, Time& duration)
{
  EGE_ASSERT(0 < duration.microseconds());

  // create effect
  PSoundEffect effect = ege_new SoundEffectFadeOut(duration);
  if (NULL != effect)
  {
    // add effect to sound
    if ( ! sound->addEffect(effect))
    {
      // error, clean up
      effect = NULL;
    }
  }

  return effect;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PSoundEffect AudioHelper::FadeOutAndStop(PSound& sound, Time& duration)
{
  EGE_ASSERT(0 < duration.microseconds());

  // create effect
  PSoundEffect effect = FadeOut(sound, duration);
  if (NULL != effect)
  {
    // connect
    ege_connect(effect, finished, sound.object(), Sound::stop); 
  }

  return effect;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END