#include "Core/Application/AppController.h"
#include "Win32/Application/AppControllerWin32_p.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(AppControllerPrivate)
EGE_DEFINE_DELETE_OPERATORS(AppControllerPrivate)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AppControllerPrivate::AppControllerPrivate(AppController* base) : m_base(base)
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
  // for Windows, we enter tight loop making sure to process all messages sent to our process
  while (AppController::STATE_QUIT != m_base->state())
  {
    MSG sMsg;

    // process messages for all windows within our context
    if (PeekMessage(&sMsg, NULL, 0, 0, PM_REMOVE)) 
    { 
      // process the message
      TranslateMessage(&sMsg);
      DispatchMessage(&sMsg);
    }

    // update
    m_base->update();

    // render
    m_base->render();
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
