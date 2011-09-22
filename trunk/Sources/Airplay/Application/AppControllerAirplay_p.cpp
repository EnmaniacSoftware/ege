#include "Core/Application/AppController.h"
#include "Airplay/Application/AppControllerAirplay_p.h"
#include "s3e.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(AppControllerPrivate)
EGE_DEFINE_DELETE_OPERATORS(AppControllerPrivate)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AppControllerPrivate::AppControllerPrivate(AppController* base) : m_d(base)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AppControllerPrivate::~AppControllerPrivate()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Enters main loop. */
EGEResult AppControllerPrivate::run()
{
  Time startTime;
  Time endTime;

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
    // TAGE - limit to 30fps
    s3eDeviceYield(Math::Max(0, 1000.0f / 30.0f - (endTime - startTime).miliseconds());
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
