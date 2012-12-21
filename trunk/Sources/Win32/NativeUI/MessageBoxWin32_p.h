#ifndef EGE_WIN32_MESSAGEBOX_PRIVATE_H
#define EGE_WIN32_MESSAGEBOX_PRIVATE_H

#include <EGE.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class MessageBox;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! @brief Private implementation for native message box for Windows. */
class MessageBoxPrivate
{
  public: 

    MessageBoxPrivate(MessageBox* base);
   ~MessageBoxPrivate();
    
    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    EGE_DECLARE_PUBLIC_IMPLEMENTATION(MessageBox)

  public:

    /*! Constructs object. */
    EGEResult construct();
    /*! Shows message box. */
    s32 show();
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_WIN32_MESSAGEBOX_PRIVATE_H