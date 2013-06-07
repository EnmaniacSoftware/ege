#ifdef EGE_AUDIO_OPENAL

#include "Core/Application/Application.h"
#include "Core/Audio/OpenAL/AudioThreadOpenAL.h"
#include "Core/Audio/OpenAL/AudioManagerOpenAL.h"
#include "EGEDevice.h"
#include "EGETime.h"
#include "EGEDebug.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define UPDATE_PERIOD (20)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AudioThreadOpenAL::AudioThreadOpenAL(Application* app, AudioManagerOpenAL* manager) : Thread(app), 
                                                                                      m_manager(manager)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AudioThreadOpenAL::~AudioThreadOpenAL()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
s32 AudioThreadOpenAL::run()
{
  Time updateTime;
  updateTime.fromMiliseconds(UPDATE_PERIOD);

  while (!isStopping())
  {   
    // update manager
    m_manager->threadUpdate(updateTime);
    
    // yield
    Device::Sleep(UPDATE_PERIOD);
  }

  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EGE_AUDIO_OPENAL
