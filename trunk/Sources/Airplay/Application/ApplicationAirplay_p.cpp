#include <EGEApplication.h>
#include "Airplay/Application/ApplicationAirplay_p.h"
#include <EGEEvent.h>
#include <EGETimer.h>
#include <EGETime.h>
#include <EGEMath.h>
#include <EGEDebug.h>
#include <s3e.h>
#include <s3eDevice.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ApplicationPrivate)
EGE_DEFINE_DELETE_OPERATORS(ApplicationPrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ApplicationPrivate::ApplicationPrivate(Application* base) : m_d(base)
{
  // register for pause/resume notifications
  if (S3E_RESULT_SUCCESS != s3eDeviceRegister(S3E_DEVICE_PAUSE, ApplicationPrivate::PauseCB, this))
  {
    egeWarning(KApplicationDebugName) << "Could not install PAUSE callback!";
  }

  if (S3E_RESULT_SUCCESS != s3eDeviceRegister(S3E_DEVICE_UNPAUSE, ApplicationPrivate::ResumeCB, this))
  {
    egeWarning(KApplicationDebugName) << "Could not install RESUME callback!";
  }
}
//------------------------------------------------------------------------  --------------------------------------------------------------------------------------
ApplicationPrivate::~ApplicationPrivate()
{
  // unregister for pause/resume notifications
  s3eDeviceUnRegister(S3E_DEVICE_PAUSE, ApplicationPrivate::PauseCB);
  s3eDeviceUnRegister(S3E_DEVICE_UNPAUSE, ApplicationPrivate::ResumeCB);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ApplicationPrivate::run()
{
  Time startTime;
  Time endTime;
  Time yieldTime;

  // app loop
  while ( ! s3eDeviceCheckQuitRequest() && (Application::STATE_QUIT != d_func()->state()))
  {
    // store this loop start time
    startTime.fromMicroseconds(Timer::GetMicroseconds());

    // send begining of frame event
    emit d_func()->frameBegin();

    // update pointer
    s3ePointerUpdate();

    // update
    d_func()->update();
   
    // render
    d_func()->render();

    // send end of frame event
    emit d_func()->frameEnd();

    // stat this loop end time
    endTime.fromMicroseconds(Timer::GetMicroseconds());

    // give some time for OS
    yieldTime = d_func()->m_renderInterval - (endTime - startTime);
    s3eDeviceYield(Math::Max((s32) 0, static_cast<s32>(yieldTime.miliseconds())));
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
int32 ApplicationPrivate::PauseCB(void* data, void* userData)
{
  ApplicationPrivate* me = static_cast<ApplicationPrivate*>(userData);

  // send event
  me->d_func()->eventManager()->send(EGE_EVENT_ID_CORE_APP_PAUSE);

  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
int32 ApplicationPrivate::ResumeCB(void* data, void* userData)
{
  ApplicationPrivate* me = static_cast<ApplicationPrivate*>(userData);

  // send event
  me->d_func()->eventManager()->send(EGE_EVENT_ID_CORE_APP_RESUME);

  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END