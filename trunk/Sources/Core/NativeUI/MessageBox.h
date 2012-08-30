#ifndef EGE_CORE_MESSAGEBOX_H
#define EGE_CORE_MESSAGEBOX_H

#include <EGEString.h>
#include <EGENativeUI.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(MessageBox, PMessageBox)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! @brief Implementation for native message box. */
class MessageBox : public Object
{
  public: 

    MessageBox(const Text& title, const Text& content, MessageBoxButtons buttons);
    virtual ~MessageBox();
    
    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /* Convinience method. Shows message box with given title and content. */
    static s32 Show(const Text& title, const Text& content, MessageBoxButtons buttons);

  public:

    /* Constructs object. */
    EGEResult construct();
    /* Shows message box. */
    s32 show();

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(MessageBox)

    /*! Title. */
    Text m_title;
    /*! Content text. */
    Text m_content;
    /*! Buttons. */
    MessageBoxButtons m_buttons;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_MESSAGEBOX_H