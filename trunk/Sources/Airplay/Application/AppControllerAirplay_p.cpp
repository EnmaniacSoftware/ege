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
  while (!s3eDeviceCheckQuitRequest() && AppController::STATE_QUIT != d_func()->state())
  {
    // update pointer
    s3ePointerUpdate();

    // update
    d_func()->update();

    // render
    d_func()->render();

    // give some time for OS
    s3eDeviceYield(0);//(int32) d_func()->m_updateInterval.miliseconds());
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
