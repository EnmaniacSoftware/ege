#include "Core/Application/Application.h"
#include "Win32/Application/ApplicationWin32_p.h"
#include <EGEEvent.h>
#include <EGETimer.h>
#include <EGETime.h>
#include <EGEMath.h>
#include <EGEDebug.h>
#include <windows.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(ApplicationPrivate)
EGE_DEFINE_DELETE_OPERATORS(ApplicationPrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ApplicationPrivate::ApplicationPrivate(Application* base) : m_d(base)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ApplicationPrivate::~ApplicationPrivate()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult ApplicationPrivate::run()
{
  Time startTime;
  Time endTime;
  Time yieldTime;

  // for Windows, we enter tight loop making sure to process all messages sent to our process
  while (Application::STATE_QUIT != d_func()->state())
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
    startTime = Timer::GetMicroseconds();

    // send begin of frame signal
    emit d_func()->frameBegin();

    // update
    d_func()->update();

    // render
    d_func()->render();

    // send end of frame signal
    emit d_func()->frameEnd();

    // stat this loop end time
    endTime = Timer::GetMicroseconds();

    // give some time for OS
    yieldTime = d_func()->m_renderInterval - (endTime - startTime);
    Sleep(Math::Max((s32) 0, static_cast<s32>(yieldTime.miliseconds())));
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END