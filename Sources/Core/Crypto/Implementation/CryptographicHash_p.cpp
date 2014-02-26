#include "Core/Crypto/Implementation/CryptographicHash_p.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(CryptographicHashPrivate)
EGE_DEFINE_DELETE_OPERATORS(CryptographicHashPrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
CryptographicHashPrivate::CryptographicHashPrivate(CryptographicHashAlgorithm algorithm) : m_algorithm(algorithm)
{
  switch (m_algorithm)
  {
    case EMD5:

      // set to proper size
      m_result.setSize(MD5_DIGEST_LENGTH);

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
    case EMD5:

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
    case EMD5:

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
    case EMD5:

      MD5_Init(&m_md5Context);
      break;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PDataBuffer CryptographicHashPrivate::result()
{
  switch (m_algorithm)
  {
    case EMD5:

      EGE_ASSERT(MD5_DIGEST_LENGTH == m_result.size());

      MD5_Final(reinterpret_cast<unsigned char*>(m_result.data(0)), &m_md5Context);
      break;
  }

  return m_result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END