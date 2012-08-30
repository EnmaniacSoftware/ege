#include "Core/Application/AppController.h"
#include "Win32/Application/AppControllerWin32_p.h"
#include <EGEDebug.h>
#include <windows.h>

EGE_NAMESPACE_BEGIN

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

  // for Windows, we enter tight loop making sure to process all messages sent to our process
  while (AppController::STATE_QUIT != d_func()->state())
  {
    MSG sMsg;

    // process messages for all windows within our context
    if (PeekMessage(&sMsg, NULL, 0, 0, PM_REMOVE)) 
    { 
      // process the message
      TranslateMessage(&sMsg);
      DispatchMessage(&sMsg);
    }

    // store this loop start time
    startTime.fromMicroseconds(Timer::GetMicroseconds());

    // update
    d_func()->update();

    // render
    d_func()->render();

    // stat this loop end time
    endTime.fromMicroseconds(Timer::GetMicroseconds());

    // give some time for OS
    yieldTime = d_func()->m_renderInterval - (endTime - startTime);
    //EGE_PRINT("%d", yieldTime.miliseconds());
    Sleep(Math::Max((s32) 0, static_cast<s32>(yieldTime.miliseconds())));
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END