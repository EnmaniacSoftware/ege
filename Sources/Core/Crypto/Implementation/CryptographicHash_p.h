#ifndef EGE_CORE_CRYPTOGRAPHICHASH_MD5_H
#define EGE_CORE_CRYPTOGRAPHICHASH_MD5_H

/** Private implementation for cryptographic hash generators. */

#include "Core/Crypto/Interface/CryptographicHash.h"
#include "Core/Crypto/Implementation/md5/md5.h"
#include "EGEDataBuffer.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class CryptographicHashPrivate
{
  public:

    CryptographicHashPrivate(CryptographicHashAlgorithm algorithm);
   ~CryptographicHashPrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    EGE_DECLARE_PUBLIC_IMPLEMENTATION(CryptographicHash)

    /*! @see ICryptographicHash::addData. */
    EGEResult addData(const char* data, s32 length);
    /*! @see ICryptographicHash::addData. */
    EGEResult addData(const PDataBuffer& data);
    /*! @see ICryptographicHash::reset. */
    void reset();
    /*! Return data buffer with result. */
    PDataBuffer result();

  private:

    /*! Algorithm to use. */
    CryptographicHashAlgorithm m_algorithm;
    /*! MD5 context. */
    MD5_CTX m_md5Context;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_CRYPTOGRAPHICHASH_MD5_H
