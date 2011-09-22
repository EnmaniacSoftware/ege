#include "Core/Application/AppController.h"
#include "Airplay/Application/AppControllerAirplay_p.h"
#include "s3e.h"
#include <EGEDebug.h>

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
    EGE_PRINT("%d", yieldTime.miliseconds());
    s3eDeviceYield(Math::Max((s32) 0, static_cast<s32>(yieldTime.miliseconds())));
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
