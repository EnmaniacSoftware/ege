#include "Core/Application/Application.h"
#include "EGEInput.h"
#include "Airplay/Input/PointerAirplay_p.h"
#include "Core/Graphics/Graphics.h"

#include "EGEString.h"
#include "EGEResources.h"
#include "EGEOverlay.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(PointerPrivate)
EGE_DEFINE_DELETE_OPERATORS(PointerPrivate)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PointerPrivate::PointerPrivate(Pointer* base) : m_base(base)
{
  bool multitouchSupported = s3ePointerGetInt(S3E_POINTER_MULTI_TOUCH_AVAILABLE) ? true : false;

  if (multitouchSupported)
  {
      s3ePointerRegister(S3E_POINTER_TOUCH_EVENT, (s3eCallback) MultiTouchButtonCB, this);
      s3ePointerRegister(S3E_POINTER_TOUCH_MOTION_EVENT, (s3eCallback) MultiTouchMotionCB, this);
  }
  else
  {
      s3ePointerRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback) SingleTouchButtonCB, this);
      s3ePointerRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback) SingleTouchMotionCB, this);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PointerPrivate::~PointerPrivate()
{
  bool multitouchSupported = s3ePointerGetInt(S3E_POINTER_MULTI_TOUCH_AVAILABLE) ? true : false;

  if (multitouchSupported)
  {
      s3ePointerUnRegister(S3E_POINTER_TOUCH_EVENT, (s3eCallback) MultiTouchButtonCB);
      s3ePointerUnRegister(S3E_POINTER_TOUCH_MOTION_EVENT, (s3eCallback) MultiTouchMotionCB);
  }
  else
  {
      s3ePointerUnRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback) SingleTouchButtonCB);
      s3ePointerUnRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback) SingleTouchMotionCB);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool PointerPrivate::isValid() const
{
  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Multitouch callback for button state changes. */
void PointerPrivate::MultiTouchButtonCB(s3ePointerTouchEvent* event, void* data)
{
  PointerPrivate* me = (PointerPrivate*) data;

  // report as left button
  EGEInput::EButton button = EGEInput::BUTTON_LEFT;

  // map state
  EGEInput::EButtonState state = (0 != event->m_Pressed) ? EGEInput::BUTTON_STATE_DOWN : EGEInput::BUTTON_STATE_UP;

  PTextOverlay overlay = me->m_base->app()->overlayManager()->overlay("pointer");
  if (NULL == overlay)
  {
    overlay = me->m_base->app()->overlayManager()->addTextOverlay("pointer");
    PResourceFont fontResource = me->m_base->app()->resourceManager()->resource("font", "ingame");
    overlay->setFont(fontResource->font());
    overlay->physics()->setPosition(TVector4f(0, 0, 0, 0));
  }
  overlay->setText(EGEText::Format("X: %d Y: %d T: %d", event->m_x, event->m_y, event->m_TouchID));

  s32 x = event->m_x;
  s32 y = event->m_y;

 // if (90.0f == me->base()->app()->graphics()->renderer()->orientationRotation().degrees())
  //{
  //  x = 480 - event->m_y;
  //  y = event->m_x;
  //}

  // send event
  me->base()->app()->eventManager()->send(EGE_EVENT_UID_CORE_POINTER_DATA, ege_new PointerData(button, state, x, y, event->m_TouchID));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Multitouch callback for motion changes. */
void PointerPrivate::MultiTouchMotionCB(s3ePointerTouchMotionEvent* event, void* data)
{
  PointerPrivate* me = (PointerPrivate*) data;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Singletouch callback for button state changes. */
void PointerPrivate::SingleTouchButtonCB(s3ePointerEvent* event, void* data)
{
  PointerPrivate* me = (PointerPrivate*) data;

  // map button
  EGEInput::EButton button;
  switch (event->m_Button)
  {
    case S3E_POINTER_BUTTON_LEFTMOUSE:      button = EGEInput::BUTTON_LEFT; break;
    case S3E_POINTER_BUTTON_RIGHTMOUSE:     button = EGEInput::BUTTON_RIGHT; break;
    case S3E_POINTER_BUTTON_MOUSEWHEELUP:   button = EGEInput::BUTTON_WHEEL_UP; break;
    case S3E_POINTER_BUTTON_MOUSEWHEELDOWN: button = EGEInput::BUTTON_WHEEL_DOWN; break;

    default:

      button = EGEInput::BUTTON_LEFT;
      break;
  }

  PTextOverlay overlay = me->m_base->app()->overlayManager()->overlay("pointer");
  if (NULL == overlay)
  {
    overlay = me->m_base->app()->overlayManager()->addTextOverlay("pointer");
    PResourceFont fontResource = me->m_base->app()->resourceManager()->resource("font", "ingame");
    overlay->setFont(fontResource->font());
    overlay->physics()->setPosition(TVector4f(0, 0, 0, 0));
  }
  overlay->setText(EGEText::Format("X: %d Y: %d", event->m_x, event->m_y));

  // map state
  EGEInput::EButtonState state = (0 != event->m_Pressed) ? EGEInput::BUTTON_STATE_DOWN : EGEInput::BUTTON_STATE_UP;

  s32 x = event->m_x;
  s32 y = event->m_y;

  //if (90.0f == me->base()->app()->graphics()->renderer()->orientationRotation().degrees())
  //{
  //  x = 480 - event->m_y;
  //  y = event->m_x;
  //}

  // send event
  me->base()->app()->eventManager()->send(EGE_EVENT_UID_CORE_POINTER_DATA, ege_new PointerData(button, state, x, y, 0));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Singletouch callback for motion changes. */
void PointerPrivate::SingleTouchMotionCB(s3ePointerMotionEvent* event, void* data)
{
  PointerPrivate* me = (PointerPrivate*) data;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
