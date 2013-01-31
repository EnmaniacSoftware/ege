#ifndef EGE_CORE_CRYPTO_CIPHER_XOR_CIPHERXORPRIVATE_H
#define EGE_CORE_CRYPTO_CIPHER_XOR_CIPHERXORPRIVATE_H

/** Private implementation for XOR cipher. */

#include "Core/Crypto/Cipher/Cipher_p.h"
#include <EGEDataBuffer.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(CipherKey, PCipherKey)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class CipherXORPrivate : public CipherPrivate
{
  public:

    CipherXORPrivate(Cipher* base, EGECipher::Direction direction, const PCipherKey& key);
    virtual ~CipherXORPrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /*! @see CipherPrivate::addData. */
    EGEResult addData(const char* data, s32 length);
    /*! @see CipherPrivate::addData. */
    EGEResult addData(const PDataBuffer& data);    
    /*! @see CipherPrivate::reset. */
    void reset();

  private:

    /*! Current index within key. */
    s32 m_keyIndex;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_CRYPTO_CIPHER_XOR_CIPHERXORPRIVATE_H
