#ifndef EGE_AIRPLAY_POINTER_PRIVATE_H
#define EGE_AIRPLAY_POINTER_PRIVATE_H

#include "EGE.h"
#include "EGEEvent.h"
#include "s3e.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Pointer;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class PointerPrivate
{
  public:

    PointerPrivate(Pointer* base);
   ~PointerPrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Returns TRUE if object is valid. */
    bool isValid() const;

  private:

    /*! Returns pointer to base object. */
    inline Pointer* base() const { return m_base; }
    /* Multitouch callback for button state changes. */
    static void MultiTouchButtonCB(s3ePointerTouchEvent* event, void* data);
    /* Multitouch callback for motion changes. */
    static void MultiTouchMotionCB(s3ePointerTouchMotionEvent* event, void* data);
    /* Singletouch callback for button state changes. */
    static void SingleTouchButtonCB(s3ePointerEvent* event, void* data);
    /* Singletouch callback for motion changes. */
    static void SingleTouchMotionCB(s3ePointerMotionEvent* event, void* data);

  private:

    /*! Pointer to base object. */
    Pointer* m_base;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_AIRPLAY_POINTER_PRIVATE_H