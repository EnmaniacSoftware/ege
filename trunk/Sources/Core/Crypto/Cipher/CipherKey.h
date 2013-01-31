#ifndef EGE_CORE_CRYPTO_CIPHER_CIPHERKEY_H
#define EGE_CORE_CRYPTO_CIPHER_CIPHERKEY_H

/** Class representing key used to cipher data. */

#include <EGEString.h>
#include <EGEDataBuffer.h>
#include <EGEDebug.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(CipherKey, PCipherKey)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class CipherKey : public Object
{
  public:

    CipherKey(const String& key);
    CipherKey(const PDataBuffer& key);
   ~CipherKey();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /*! Returns key value at given index. */
    u8 value(s32 index) const;
    /*! Returns length of the key (in bytes). */
    s64 length() const;

  private:

    /*! Key data. */
    DataBuffer m_key;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline u8 CipherKey::value(s32 index) const
{
  EGE_ASSERT(index < m_key.size());

  return *reinterpret_cast<u8*>(m_key.data(static_cast<s64>(index)));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline s64 CipherKey::length() const
{
  return m_key.size();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_CRYPTO_CIPHER_CIPHERKEY_H
