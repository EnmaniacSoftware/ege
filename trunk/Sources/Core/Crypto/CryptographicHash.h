#ifndef EGE_CORE_CRYPTOGRAPHICHASH_H
#define EGE_CORE_CRYPTOGRAPHICHASH_H

/** Class providing the way to generate cryptographic hashes. */

#include "EGEDataBuffer.h"
#include "EGEHash.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class CryptographicHash
{
  public:

    CryptographicHash(EGECryptographicHash::Algorithm algorithm);
   ~CryptographicHash();

    /*! Returns TRUE if object is valid. */
    bool isValid() const;
    /*! Adds data from raw buffer to cryptographic hash. */
    EGEResult addData(const char* data, s32 length);
    /*! Adds data from buffer to cryptographic hash. */
    EGEResult addData(const PDataBuffer& data);
    /*! Resets object. */
    void reset();
    /*! Return data buffer with result. */
    PDataBuffer result();

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(CryptographicHash)
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_CRYPTOGRAPHICHASH_H
