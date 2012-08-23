#include "Core/Social/SocialPlatform.h"
#include "Core/Social/SocialPlatformNull_p.h"

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
  return EGE_ERROR_NOT_SUPPORTED;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END