#include "Core/Social/SocialPlatform.h"
#include "Airplay/Social/SocialPlatformGameCenterAirplay_p.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(SocialPlatformPrivate)
EGE_DEFINE_DELETE_OPERATORS(SocialPlatformPrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SocialPlatformPrivate::SocialPlatformPrivate(SocialPlatform* base) : m_d(base)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SocialPlatformPrivate::~SocialPlatformPrivate()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Constructs object. */
EGEResult SocialPlatformPrivate::construct()
{
  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Starts authentication. */
EGEResult SocialPlatformPrivate::startAuthentication()
{
  // check if supported
  if ( ! s3eIOSGameCenterAvailable())
  {
    // error!
    return EGE_ERROR_NOT_SUPPORTED;
  }

  // check if authenticated already
  if (s3eIOSGameCenterGetInt(S3E_IOSGAMECENTER_LOCAL_PLAYER_IS_AUTHENTICATED))
  {
    // done
    // TAGE - should be emit ?
    return EGE_SUCCESS;
  }

  // start authentication
  return (S3E_RESULT_SUCCESS == s3eIOSGameCenterAuthenticate(AuthenticationCallback, this)) ? EGE_SUCCESS : EGE_ERROR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Authentication callback. */
void SocialPlatformPrivate::AuthenticationCallback(s3eIOSGameCenterError* error, void* userData)
{
  SocialPlatformPrivate* me = (SocialPlatformPrivate*) userData;

  EGEResult result = EGE_ERROR;

  // map error code to framework values
  switch (*error)
  {
    case S3E_IOSGAMECENTER_ERR_NONE:        result = EGE_SUCCESS; break;
    case S3E_IOSGAMECENTER_ERR_UNSUPPORTED: result = EGE_ERROR_NOT_SUPPORTED; break;
  }

  // emit
  emit me->d_func()->authenticated(result);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END