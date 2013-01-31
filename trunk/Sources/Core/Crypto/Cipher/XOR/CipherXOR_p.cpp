#include "Core/Crypto/Cipher/XOR/CipherXOR_p.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(CipherXORPrivate)
EGE_DEFINE_DELETE_OPERATORS(CipherXORPrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
CipherXORPrivate::CipherXORPrivate(Cipher* base, EGECipher::Direction direction, const PCipherKey& key) : CipherPrivate(base, direction, key),
                                                                                                          m_keyIndex(0)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
CipherXORPrivate::~CipherXORPrivate()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult CipherXORPrivate::addData(const char* data, s32 length)
{
  // make sure there is space in result buffer
  EGEResult result = m_result->setSize(m_result->writeOffset() + length);
  if (EGE_SUCCESS == result)
  {
    // get pointer to first element in result buffer where new data should be written to
    char* outData = reinterpret_cast<char*>(m_result->data(m_result->writeOffset()));

    while (0 < length)
    {
      // encrypt/decrypt
      *outData = *data ^ m_key->value(m_keyIndex);

      // next
      ++outData;
      ++data;
      ++m_keyIndex;
      --length;
    }
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult CipherXORPrivate::addData(const PDataBuffer& data)
{
  return addData(reinterpret_cast<const char*>(data->data(data->readOffset())), static_cast<s32>(data->size() - data->readOffset()));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void CipherXORPrivate::reset()
{
  // reset index
  m_keyIndex = 0;

  // call base class
  CipherPrivate::reset();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END