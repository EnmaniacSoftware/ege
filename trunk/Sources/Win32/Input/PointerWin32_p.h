#ifndef EGE_WIN32_POINTER_PRIVATE_H
#define EGE_WIN32_POINTER_PRIVATE_H

#include <EGE.h>
#include <EGEEvent.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Pointer;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class PointerPrivate : public IEventListener
{
  public:

    PointerPrivate(Pointer* base);
   ~PointerPrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Returns TRUE if object is valid. */
    bool isValid() const;

  private:

    /* IEventListener override. Event reciever. */
    void onEventRecieved(PEvent event) override;
    /*! Returns pointer to base object. */
    inline Pointer* base() const { return m_base; }

  private:

    /*! Pointer to base object. */
    Pointer* m_base;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_WIN32_POINTER_PRIVATE_H