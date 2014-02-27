#ifndef EGE_CORE_CRYPTO_CIPHER_CIPHER_H
#define EGE_CORE_CRYPTO_CIPHER_CIPHER_H

/** Base class for objects capable of encrypting/decrypting data. */

#include "EGEDataBuffer.h"
#include "Core/Crypto/Interface/Cipher/CipherKey.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Available ciphering directions. */
enum CipherDirection
{
  EEncrypt = 0,
  EDecrypt
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(Cipher, PCipher)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Cipher : public Object
{
  public:

    Cipher(CipherDirection direction, const PCipherKey& key);
    virtual ~Cipher();

    /*! Adds data from raw buffer for ciphring. */
    virtual EGEResult addData(const char* data, s32 length) = 0;
    /*! Adds data from buffer for ciphring. */
    EGEResult addData(const PDataBuffer& data);
    /*! Resets object. */
    virtual void reset();
    /*! Return data buffer with result. */
    PDataBuffer result();

  protected:

    /*! Cipher key. */
    PCipherKey m_key;
    /*! Direction. */
    CipherDirection m_direction;
    /*! Result buffer. */
    DataBuffer m_result;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_CRYPTO_CIPHER_CIPHER_H
