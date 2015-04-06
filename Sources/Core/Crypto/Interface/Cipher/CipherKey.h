#ifndef EGE_CORE_CRYPTO_CIPHER_CIPHERKEY_H
#define EGE_CORE_CRYPTO_CIPHER_CIPHERKEY_H

/** Class representing key used to cipher data. */

#include "EGEString.h"
#include "EGEDataBuffer.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_PREDECLARE_SMART_CLASS(CipherKey, PCipherKey)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class CipherKey : public Object
{
  public:

    CipherKey(const String& key);
    CipherKey(const PDataBuffer& key);
   ~CipherKey();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /*! Returns pointer to key data buffer. */
    u8* data() const;
    /*! Returns length of the key (in bytes). */
    s64 length() const;

  private:

    /*! Key data. */
    DataBuffer m_key;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_CRYPTO_CIPHER_CIPHERKEY_H
