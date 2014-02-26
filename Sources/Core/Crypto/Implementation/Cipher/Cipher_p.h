#ifndef EGE_CORE_CRYPTO_CIPHER_CIPHERPRIVATE_H
#define EGE_CORE_CRYPTO_CIPHER_CIPHERPRIVATE_H

/** Private implementation interface for ciphers. */

#include "EGEDataBuffer.h"
#include "Core/Crypto/Interface/Cipher/CipherKey.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Cipher;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class CipherPrivate
{
  public:

    CipherPrivate(Cipher* base, const PCipherKey& key);
    virtual ~CipherPrivate();

    EGE_DECLARE_PUBLIC_IMPLEMENTATION(Cipher)

    /*! @see Cipher::isValid. */
    virtual bool isValid() const;
    /*! @see Cipher::addData. */
    virtual EGEResult addData(const char* data, s32 length) = 0;
    /*! @see Cipher::addData. */
    virtual EGEResult addData(const PDataBuffer& data) = 0;    
    /*! @see Cipher::reset. */
    virtual void reset();
    /*! @see Cipher::result. */
    PDataBuffer result();

  protected:

    /*! Cipher key. */
    PCipherKey m_key; 
    /*! Result buffer. */
    PDataBuffer m_result;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_CRYPTO_CIPHER_CIPHERPRIVATE_H
