#ifndef EGE_CORE_CRYPTO_CRYPTOGRAPHICHASH_H
#define EGE_CORE_CRYPTO_CRYPTOGRAPHICHASH_H

/** Base class for objects providing the way to generate cryptographic hashes. */

#include "EGEDataBuffer.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class CryptographicHash
{
  public:

    CryptographicHash();
    virtual ~CryptographicHash();

    /*! Adds data from raw buffer to cryptographic hash. */
    virtual EGEResult addData(const char* data, s32 length) = 0;
    /*! Adds data from buffer to cryptographic hash. */
    EGEResult addData(const PDataBuffer& data);
    /*! Resets object. */
    virtual void reset();
    /*! Return data buffer with result. */
    virtual PDataBuffer result();

  protected:

    /*! Result buffer. */
    DataBuffer m_result;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_CRYPTO_CRYPTOGRAPHICHASH_H
