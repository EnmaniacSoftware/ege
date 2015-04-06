#ifndef EGE_CORE_POINTEREVENT_H
#define EGE_CORE_POINTEREVENT_H

/*! This class represents generic pointer event. */

#include "EGE.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Available actions. */
enum PointerAction
{
  EPointerActionDown = 0,
  EPointerActionUp,
  EPointerActionMove
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_PREDECLARE_SMART_CLASS(PointerEvent, PPointerEvent)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class PointerEvent : public Object
{
  public:

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! Returns action type. */
    PointerAction action() const;
    /*! Returns X position. */
    s32 x() const;
    /*! Returns Y position. */
    s32 y() const;

  protected:

    PointerEvent(PointerAction action, s32 x, s32 y, u32 uid);
   ~PointerEvent();

  private:

    /*! X position of event. */
    s32 m_x;
    /*! Y position of event. */
    s32 m_y;
    /*! Action type. */
    PointerAction m_action;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_POINTEREVENT_H