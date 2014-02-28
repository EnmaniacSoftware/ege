#include "Core/Crypto/Interface/Cipher/CipherAES.h"
#include "Core/Crypto/Interface/Cipher/CipherKey.h"
#include "EGEDebug.h"
#include <openssl/rand.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(CipherAES)
EGE_DEFINE_DELETE_OPERATORS(CipherAES)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
CipherAES::CipherAES(CipherDirection direction, const PCipherKey& key) : Cipher(direction, key)
{
  // reset initialization vector
  RAND_bytes(m_initializationVector, AES_BLOCK_SIZE);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
CipherAES::~CipherAES()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult CipherAES::addData(const char* data, s32 length)
{
  EGE_ASSERT(128 == (m_key->length() << 3));

  EGEResult result = EGE_SUCCESS;

  // check if encryping
  if (EEncrypt == m_direction)
  {
    // prepare encryption key
    AES_KEY key;
    AES_set_encrypt_key(key, m_key->size() >> 3, m_key->data());

    // calculate size of encoded output
    const s64 encodedSize = ((length + AES_BLOCK_SIZE) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;

    // make sure buffer is big enough
    result = m_result.setSize(m_result.size() + encodedSize);

    if (EGE_SUCCESS == result)
    {
      // encrypt data
      AES_cbc_encrypt(data, m_result.data(m_result.writeOffset()), length, &key, m_initializationVector, AES_ENCRYPT);
    }
  }
  else
  {
    // prepare encryption key
    AES_KEY key;
    AES_set_decrypt_key(key, m_key->size() >> 3, m_key->data());

    if (EGE_SUCCESS == result)
    {
      // encrypt data
//      AES_cbc_encrypt(enc_out, dec_out, encslength, &key, m_initializationVector, AES_DECRYPT);
    }
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void CipherAES::reset()
{
  // reset initialization vector
  RAND_bytes(m_initializationVector, AES_BLOCK_SIZE);

  // call base class
  Cipher::reset();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
