#ifndef EGE_CORE_SOCIALPLATFORM_PRIVATE_H
#define EGE_CORE_SOCIALPLATFORM_PRIVATE_H

#include <EGE.h>
#include <s3eIOSGameCenter.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class SocialPlatform;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! \brief Game Center implementation for social platform. */
class SocialPlatformPrivate
{
  public:
    
    SocialPlatformPrivate(SocialPlatform* base);
   ~SocialPlatformPrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    EGE_DECLARE_PUBLIC_IMPLEMENTATION(SocialPlatform)

  public:

    /* Constructs object. */
    EGEResult construct();
    /* Starts authentication. */
    EGEResult startAuthentication();

  private:

    /* Authentication callback. */
    static void AuthenticationCallback(s3eIOSGameCenterError* error, void* userData);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_SOCIALPLATFORM_PRIVATE_H