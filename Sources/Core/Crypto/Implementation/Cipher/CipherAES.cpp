#include "Core/Crypto/Interface/Cipher/CipherAES.h"
#include "Core/Crypto/Interface/Cipher/CipherKey.h"
#include "EGEDataStream.h"
#include "EGEDebug.h"
#include "EGERandom.h"
#include <openssl/aes.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(CipherAES)
EGE_DEFINE_DELETE_OPERATORS(CipherAES)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
CipherAES::CipherAES(CipherDirection direction, const PCipherKey& key) 
: Cipher(direction, key)
{
  EGE_ASSERT(AES_BLOCK_SIZE == sizeof (m_initializationVector));
 
  resetInitializationVectors(); 
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

  // determine if this is first operation
  bool firstOperation = (0 == m_result.size());

  // check if encryping
  if (EEncrypt == m_direction)
  {
    // prepare encryption key
    AES_set_encrypt_key(m_key->data(), static_cast<int>(m_key->length() << 3), &key);

    // calculate size of encoded output
    const s32 encodedSize = ((length + AES_BLOCK_SIZE) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;

    // make sure buffer is big enough
    const s64 requiredSize = m_result.size() + encodedSize + KDataBlockLengthSize + (firstOperation ? AES_BLOCK_SIZE : 0);
    result = m_result.setSize(requiredSize);

    // check if first operation
    if (firstOperation && (EGE_SUCCESS == result))
    {
      // write initialization vector first
      result = (AES_BLOCK_SIZE == m_result.write(m_initializationVector, AES_BLOCK_SIZE)) ? EGE_SUCCESS : EGE_ERROR_IO;
    }

    // next, write length of data
    DataStream stream(&m_result);
    stream << length;

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
    EGE_ASSERT_X(KDataBlockLengthSize <= length, "Too little data! Not even data block size could fit in!!!");

    // prepare encryption key
    AES_set_decrypt_key(m_key->data(), static_cast<int>(m_key->length() << 3), &key);

    // check if first operation
    if (firstOperation)
    {
      // read data length of next block
      DataBuffer buffer(reinterpret_cast<const void*>(data), AES_BLOCK_SIZE);

      // read initialization vector first
      result = (AES_BLOCK_SIZE == buffer.read(m_initializationVector, AES_BLOCK_SIZE)) ? EGE_SUCCESS : EGE_ERROR_IO;
      if (EGE_SUCCESS == result)
      {
        // move the data pointer
        data += AES_BLOCK_SIZE;
        length -= AES_BLOCK_SIZE;
      }
    }

    // read all data blocks
    while ((EGE_SUCCESS == result) && (0 < length))
    {
      // read data length of next block
      DataBuffer buffer(reinterpret_cast<const void*>(data), KDataBlockLengthSize);
      DataStream stream(&buffer);

      s32 dataBlockLength;
      stream >> dataBlockLength;

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
    }
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void CipherAES::reset()
{
  // reset initialization vectors
  resetInitializationVectors();

  // call base class
  Cipher::reset();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void CipherAES::resetInitializationVectors()
{
  RandomGenerator random;
  
  for (s32 i = 0; i < AES_BLOCK_SIZE; ++i)
  {
    m_initializationVector[i] = static_cast<u8>(random() & 0xff);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END