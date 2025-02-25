#include "Core/Crypto/Interface/Cipher/Cipher.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Cipher::Cipher(CipherDirection direction, const PCipherKey& key) : Object(NULL)
                                                                 , m_key(key)
                                                                 , m_direction(direction)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Cipher::~Cipher()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult Cipher::addData(const PDataBuffer& data)
{
  return addData(reinterpret_cast<const char*>(data->data(data->readOffset())), static_cast<s32>(data->size() - data->readOffset()));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Cipher::reset()
{
  m_result.clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PDataBuffer Cipher::result()
{
  return m_result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
