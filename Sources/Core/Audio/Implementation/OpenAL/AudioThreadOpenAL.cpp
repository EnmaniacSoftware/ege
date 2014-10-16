#include "Core/Audio/Implementation/OpenAL/AudioThreadOpenAL.h"
#include "Core/Audio/Interface/OpenAL/AudioManagerOpenAL.h"
#include "EGEDevice.h"
#include "EGETime.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define UPDATE_PERIOD (20)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AudioThreadOpenAL::AudioThreadOpenAL(AudioManagerOpenAL* manager) 
: Thread()
, m_manager(manager)
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

  while ( ! isStopping())
  {   
    // update manager
    m_manager->threadUpdate(updateTime);
    
    // yield
    Device::Sleep(UPDATE_PERIOD);
  }

  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END
