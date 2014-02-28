#ifndef EGE_CORE_CRYPTO_CIPHER_CIPHERAES_H
#define EGE_CORE_CRYPTO_CIPHER_CIPHERAES_H

/** Class providing the way to encrypt/decrypt data using Advanced Encryption Standard specification. */

#include "Core/Crypto/Interface/Cipher/Cipher.h"
#include <openssl/aes.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(CipherKey, PCipherKey)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class CipherAES : public Cipher
{
  public:

    CipherAES(CipherDirection direction, const PCipherKey& key);
   ~CipherAES();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! Allow overloads from base class. */
    using Cipher::addData;
 
  public:

    /*! @see Cipher::addData. */
    EGEResult addData(const char* data, s32 length) override;
    /*! @see Cipher::reset. */
    void reset() override;

  private:

    /*! Initialization vector. */
    u8 m_initializationVector[AES_BLOCK_SIZE];
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_CRYPTO_CIPHER_CIPHERAES_H
