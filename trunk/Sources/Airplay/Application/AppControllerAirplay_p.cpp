#include "Core/Application/AppController.h"
#include "Airplay/Application/AppControllerAirplay_p.h"
#include <s3e.h>
#include <s3eDevice.h>
#include <EGEDebug.h>
#include <EGEEvent.h>
#include <EGEApplication.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(AppControllerPrivate)
EGE_DEFINE_DELETE_OPERATORS(AppControllerPrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AppControllerPrivate::AppControllerPrivate(AppController* base) : m_d(base)
{
  // register for pause/resume notifications
  if (S3E_RESULT_SUCCESS != s3eDeviceRegister(S3E_DEVICE_PAUSE, AppControllerPrivate::PauseCB, this))
  {
    EGE_WARNING("Could not install PAUSE callback!");
  }

  if (S3E_RESULT_SUCCESS != s3eDeviceRegister(S3E_DEVICE_UNPAUSE, AppControllerPrivate::ResumeCB, this))
  {
    EGE_WARNING("Could not install RESUME callback!");
  }
}
//------------------------------------------------------------------------  --------------------------------------------------------------------------------------
AppControllerPrivate::~AppControllerPrivate()
{
  // unregister for pause/resume notifications
  s3eDeviceUnRegister(S3E_DEVICE_PAUSE, AppControllerPrivate::PauseCB);
  s3eDeviceUnRegister(S3E_DEVICE_UNPAUSE, AppControllerPrivate::ResumeCB);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Enters main loop. */
EGEResult AppControllerPrivate::run()
{
  Time startTime;
  Time endTime;
  Time yieldTime;

  // app loop
  while (!s3eDeviceCheckQuitRequest() && AppController::STATE_QUIT != d_func()->state())
  {
    // store this loop start time
    startTime.fromMicroseconds(d_func()->timer()->microseconds());

    // update pointer
    s3ePointerUpdate();

    // update
    d_func()->update();

    // render
    d_func()->render();

    // stat this loop end time
    endTime.fromMicroseconds(d_func()->timer()->microseconds());

    // give some time for OS
    yieldTime = d_func()->m_renderInterval - (endTime - startTime);
    //EGE_PRINT("%d", yieldTime.miliseconds());
    s3eDeviceYield(Math::Max((s32) 0, static_cast<s32>(yieldTime.miliseconds())));
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Pause callback. */
int32 AppControllerPrivate::PauseCB(void* data, void* userData)
{
  AppControllerPrivate* me = static_cast<AppControllerPrivate*>(userData);

  // send event
  me->d_func()->app()->eventManager()->send(EGE_EVENT_ID_CORE_APP_PAUSE);

  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Resume callback. */
int32 AppControllerPrivate::ResumeCB(void* data, void* userData)
{
  AppControllerPrivate* me = static_cast<AppControllerPrivate*>(userData);

  // send event
  me->d_func()->app()->eventManager()->send(EGE_EVENT_ID_CORE_APP_RESUME);

  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
