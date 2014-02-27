#ifndef EGE_CORE_CRYPTO_CRYPTOGRAPHICHASHMD5_H
#define EGE_CORE_CRYPTO_CRYPTOGRAPHICHASHMD5_H

/** Class providing MD5 cryptographic hashes. */

#include "Core/Crypto/Interface/CryptographicHash.h"
#include <openssl/md5.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class CryptographicHashMD5 : public CryptographicHash
{
  public:

    CryptographicHashMD5();
   ~CryptographicHashMD5();

  public:

    /*! Allow overloads from base class. */
    using CryptographicHash::addData;
 
  public:

    /*! @see CryptographicHash::addData. */
    EGEResult addData(const char* data, s32 length) override;
    /*! @see CryptographicHash::reset. */
    void reset() override;
    /*! @see CryptographicHash::reset. */
    PDataBuffer result() override;

  private:

    /*! MD5 context. */
    MD5_CTX m_context;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_CRYPTO_CRYPTOGRAPHICHASHMD5_H
