#ifndef EGE_CORE_MESSAGEBOX_H
#define EGE_CORE_MESSAGEBOX_H

#include <EGEString.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(MessageBox, PMessageBox)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! @brief Implementation for native message box. */
class MessageBox : public Object
{
  public: 

    MessageBox(const Text& title, const Text& content);
    virtual ~MessageBox();
    
    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /* Convinience method. Shows message box with given title and content. */
    static void Show(const Text& title, const Text& content);

  public:

    /* Constructs object. */
    EGEResult construct();
    /* Shows message box. */
    void show();

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(MessageBox)

    /*! Title. */
    Text m_title;
    /*! Content text. */
    Text m_content;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_MESSAGEBOX_H