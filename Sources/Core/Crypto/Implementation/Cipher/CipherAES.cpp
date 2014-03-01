#include "Core/Crypto/Interface/Cipher/CipherAES.h"
#include "Core/Crypto/Interface/Cipher/CipherKey.h"
#include "EGEDebug.h"
#include <openssl/rand.h>
#include <openssl/aes.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(CipherAES)
EGE_DEFINE_DELETE_OPERATORS(CipherAES)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
CipherAES::CipherAES(CipherDirection direction, const PCipherKey& key) : Cipher(direction, key)
{
  EGE_ASSERT(AES_BLOCK_SIZE == sizeof (m_initializationVector));

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

  const s32 KDataBlockLengthSize = 4;

  EGEResult result = EGE_SUCCESS;

  AES_KEY key;

  // check if encryping
  if (EEncrypt == m_direction)
  {
    // prepare encryption key
    AES_set_encrypt_key(m_key->data(), static_cast<int>(m_key->length() << 3), &key);

    // calculate size of encoded output
    const s32 encodedSize = ((length + AES_BLOCK_SIZE) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;

    // make sure buffer is big enough
    result = m_result.setSize(m_result.size() + encodedSize + KDataBlockLengthSize);

    // write length of data first
    m_result << length;

    if (EGE_SUCCESS == result)
    {
      // encrypt data
      AES_cbc_encrypt(reinterpret_cast<const unsigned char*>(data), reinterpret_cast<unsigned char*>(m_result.data(m_result.writeOffset())), encodedSize, &key, 
                      m_initializationVector, AES_ENCRYPT);

      // move write offset
      m_result.setWriteOffset(m_result.writeOffset() + encodedSize);
    }
  }
  else
  {
    EGE_ASSERT_X(4 <= length, "Too little data! Not even data block size could fit in!!!");

    // prepare encryption key
    AES_set_decrypt_key(m_key->data(), static_cast<int>(m_key->length() << 3), &key);

    // read all data blocks
    do
    {
      // read data length of next block
      DataBuffer buffer(reinterpret_cast<const void*>(data), KDataBlockLengthSize);

      s32 dataBlockLength;
      buffer >> dataBlockLength;

      // make sure buffer is big enough
      result = m_result.setSize(m_result.size() + dataBlockLength);

      if (EGE_SUCCESS == result)
      {
        // calculate size of encoded output
        const s32 encodedSize = ((dataBlockLength + AES_BLOCK_SIZE) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;

        // encrypt data
        AES_cbc_encrypt(reinterpret_cast<const unsigned char*>(data + KDataBlockLengthSize), 
                        reinterpret_cast<unsigned char*>(m_result.data(m_result.writeOffset())), dataBlockLength, &key, m_initializationVector, AES_DECRYPT);

        // move write offset
        m_result.setWriteOffset(m_result.writeOffset() + dataBlockLength);

        // advance over process data
        data += KDataBlockLengthSize + encodedSize;
        length -= KDataBlockLengthSize + encodedSize;
      }

    } while ((EGE_SUCCESS == result) && (0 < length));
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
