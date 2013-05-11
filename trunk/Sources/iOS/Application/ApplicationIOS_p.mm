#include "Core/Application/Application.h"
#include "iOS/Application/ApplicationIOS_p.h"
#include "EGEEvent.h"
#include "EGETimer.h"
#include "EGETime.h"
#include "EGEMath.h"
#include "EGEDebug.h"
#import "iOS/Application/AppDelegate.h"

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
  // check if running
  if (Application::STATE_QUIT != d_func()->state())
  {
    // emit frame begin
    emit d_func()->frameBegin();
    
    // update
    d_func()->update();
   
    // render
    d_func()->render();

    // emit frame end
    emit d_func()->frameEnd();
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//int32 ApplicationPrivate::PauseCB(void* data, void* userData)
//{
//  ApplicationPrivate* me = static_cast<ApplicationPrivate*>(userData);
//
//  // send event
//  me->d_func()->eventManager()->send(EGE_EVENT_ID_CORE_APP_PAUSE);
//
//  return 0;
//}
////--------------------------------------------------------------------------------------------------------------------------------------------------------------
//int32 ApplicationPrivate::ResumeCB(void* data, void* userData)
//{
//  ApplicationPrivate* me = static_cast<ApplicationPrivate*>(userData);
//
//  // send event
//  me->d_func()->eventManager()->send(EGE_EVENT_ID_CORE_APP_RESUME);
//
//  return 0;
//}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END