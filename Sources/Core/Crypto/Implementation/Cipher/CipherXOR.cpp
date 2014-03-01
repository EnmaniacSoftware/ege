#include "Core/Crypto/Interface/Cipher/CipherXOR.h"
#include "Core/Crypto/Interface/Cipher/CipherKey.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(CipherXOR)
EGE_DEFINE_DELETE_OPERATORS(CipherXOR)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
CipherXOR::CipherXOR(CipherDirection direction, const PCipherKey& key) : Cipher(direction, key)
                                                                       , m_keyIndex(0)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
CipherXOR::~CipherXOR()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult CipherXOR::addData(const char* data, s32 length)
{
  // make sure there is space in result buffer
  EGEResult result = m_result.setSize(m_result.writeOffset() + length);
  if (EGE_SUCCESS == result)
  {
    // get pointer to first element in result buffer where new data should be written to
    char* outData = reinterpret_cast<char*>(m_result.data(m_result.writeOffset()));

    // update write offset already
    m_result.setWriteOffset(m_result.writeOffset() + length);

    while (0 < length)
    {
      // encrypt/decrypt
      *outData = *data ^ m_key->data()[m_keyIndex];

      // next
      ++outData;
      ++data;
      m_keyIndex = (m_keyIndex + 1) % m_key->length();
      --length;
    }
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void CipherXOR::reset()
{
  // reset index
  m_keyIndex = 0;

  // call base class
  Cipher::reset();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
