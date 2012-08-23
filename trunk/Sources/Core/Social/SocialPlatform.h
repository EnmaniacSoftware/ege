#ifndef EGE_CORE_SOCIALPLATFORM_H
#define EGE_CORE_SOCIALPLATFORM_H

#include <EGE.h>
#include <EGESignal.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(SocialPlatform, PSocialPlatform)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! \brief Class representing social platform. */
class SocialPlatform : public Object
{
  public:
  
    SocialPlatform(Application* app);
    virtual ~SocialPlatform();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  signals:

    /*! Signal emitted when authentication is done. 
        @param result Result code of authentication.
     */
    Signal1<EGEResult> authenticated;
    
  public:

    /* Constructs object. */
    EGEResult construct();
    /* Starts authentication. */
    EGEResult startAuthentication();

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(SocialPlatform)
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_SOCIALPLATFORM_H