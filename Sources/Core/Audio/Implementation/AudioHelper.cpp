#include "EGEEngine.h"
#include "EGEResources.h"
#include "Core/Audio/Interface/AudioHelper.h"
#include "Core/Audio/Interface/Sound.h"
#include "Core/Audio/Interface/Effects/SoundEffectFadeIn.h"
#include "Core/Audio/Interface/Effects/SoundEffectFadeOut.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PSound AudioHelper::CreateSound(Engine& engine, const String& name, const String& resourceGroupName)
{
  PSound sound;

  // find sound resource
  PResourceSound soundResource = engine.resourceManager()->soundResource(name, resourceGroupName);
  if (NULL != soundResource)
  {
    // create sound instance
    sound = soundResource->createInstance();
  }

  return sound;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PSound AudioHelper::CreateSoundAndPlay(Engine& engine, const String& name, s32 repeatCount, const String& resourceGroupName)
{
  // create sound
  PSound sound = CreateSound(engine, name, resourceGroupName);
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
PSoundEffect AudioHelper::FadeIn(PSound& sound, const Time& duration)
{
  EGE_ASSERT(0 < duration.microseconds());
  EGE_ASSERT(NULL != sound);

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
PSoundEffect AudioHelper::FadeOut(PSound& sound, const Time& duration)
{
  EGE_ASSERT(0 < duration.microseconds());
  EGE_ASSERT(NULL != sound);

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
PSoundEffect AudioHelper::FadeOutAndStop(PSound& sound, const Time& duration)
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