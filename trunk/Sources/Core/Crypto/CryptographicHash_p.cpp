#include "Core/Crypto/CryptographicHash_p.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
CryptographicHashPrivate::CryptographicHashPrivate(EGECryptographicHash::Algorithm algorithm) : m_algorithm(algorithm)
{
  switch (m_algorithm)
  {
    case EGECryptographicHash::ALGORITHM_MD5:

      MD5_Init(&m_md5Context);
      break;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
CryptographicHashPrivate::~CryptographicHashPrivate()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult CryptographicHashPrivate::addData(const char* data, s32 length)
{
  switch (m_algorithm)
  {
    case EGECryptographicHash::ALGORITHM_MD5:

      MD5_Update(&m_md5Context, const_cast<void*>(reinterpret_cast<const void*>(data)), static_cast<unsigned long>(length));
      return EGE_SUCCESS;
  }

  return EGE_ERROR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult CryptographicHashPrivate::addData(const PDataBuffer& data)
{
  switch (m_algorithm)
  {
    case EGECryptographicHash::ALGORITHM_MD5:

      MD5_Update(&m_md5Context, data->data(), static_cast<unsigned long>(data->size()));
      return EGE_SUCCESS;
  }

  return EGE_ERROR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void CryptographicHashPrivate::reset()
{
  switch (m_algorithm)
  {
    case EGECryptographicHash::ALGORITHM_MD5:

      MD5_Init(&m_md5Context);
      break;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PDataBuffer CryptographicHashPrivate::result()
{
  PDataBuffer buffer;

  switch (m_algorithm)
  {
    case EGECryptographicHash::ALGORITHM_MD5:

      if (EGE_SUCCESS == buffer->setSize(16))
      {
        MD5_Final(reinterpret_cast<unsigned char*>(buffer->data(0)), &m_md5Context);
      }
      break;
  }

  return buffer;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END