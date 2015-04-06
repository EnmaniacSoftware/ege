#ifndef EGE_CORE_CRYPTO_CIPHER_CIPHERXOR_H
#define EGE_CORE_CRYPTO_CIPHER_CIPHERXOR_H

/** Class providing the way to encrypt/decrypt data. */

#include "Core/Crypto/Interface/Cipher/Cipher.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_PREDECLARE_SMART_CLASS(CipherKey, PCipherKey)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class CipherXOR : public Cipher
{
  public:

    CipherXOR(CipherDirection direction, const PCipherKey& key);
   ~CipherXOR();

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

    /*! Current index within key. */
    s32 m_keyIndex;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_CRYPTO_CIPHER_CIPHERXOR_H
