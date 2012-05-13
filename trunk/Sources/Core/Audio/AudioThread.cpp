#include "Core/Application/Application.h"
#include "Core/Audio/AudioThread.h"
#include "Core/Audio/AudioManager.h"
#include <EGEDevice.h>
#include <EGETime.h>
#include <EGEDebug.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define UPDATE_PERIOD (20)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AudioThread::AudioThread(Application* app) : Thread(app)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AudioThread::~AudioThread()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Thread override. Work method. */
s32 AudioThread::run()
{
  AudioManager* manager = app()->audioManager();

  Time updateTime;
  updateTime.fromMiliseconds(UPDATE_PERIOD);

  while (!isStopping())
  {
    manager->m_mutex->lock();
    manager->update(updateTime);
    manager->m_mutex->unlock();

    // yield
    Device::Sleep(UPDATE_PERIOD);
  }

  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
