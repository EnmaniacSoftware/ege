#ifndef EGE_CORE_CRYPTO_CIPHER_CIPHER_H
#define EGE_CORE_CRYPTO_CIPHER_CIPHER_H

/** Class providing the way to encrypt/decrypt data. */

#include "EGEDataBuffer.h"
#include "EGECipher.h"
#include "Core/Crypto/Cipher/CipherKey.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Cipher
{
  public:

    Cipher(EGECipher::Algorithm algorithm, EGECipher::Direction direction, const PCipherKey& key);
   ~Cipher();

    /*! Returns TRUE if object is valid. */
    bool isValid() const;
    /*! Adds data from raw buffer for ciphring. */
    EGEResult addData(const char* data, s32 length);
    /*! Adds data from buffer for ciphring. */
    EGEResult addData(const PDataBuffer& data);
    /*! Resets object. */
    void reset();
    /*! Return data buffer with result. */
    PDataBuffer result();

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(Cipher)
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_CRYPTO_CIPHER_CIPHER_H
