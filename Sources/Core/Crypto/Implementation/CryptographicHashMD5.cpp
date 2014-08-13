#include "Core/Crypto/Interface/CryptographicHashMD5.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
CryptographicHashMD5::CryptographicHashMD5() : CryptographicHash()
{
  // set to proper size
  m_result.setSize(MD5_DIGEST_LENGTH);

  // initialize
  MD5_Init(&m_context);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
CryptographicHashMD5::~CryptographicHashMD5()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult CryptographicHashMD5::addData(const char* data, s32 length)
{
  MD5_Update(&m_context, const_cast<void*>(reinterpret_cast<const void*>(data)), static_cast<unsigned long>(length));

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void CryptographicHashMD5::reset()
{
  // call base class
  CryptographicHash::reset();

  // re-initialize context
  MD5_Init(&m_context);

  // set to proper size
  m_result.setSize(MD5_DIGEST_LENGTH);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PDataBuffer CryptographicHashMD5::result()
{
  EGE_ASSERT(MD5_DIGEST_LENGTH == m_result.size());

  MD5_Final(reinterpret_cast<unsigned char*>(m_result.data(0)), &m_context);

  return CryptographicHash::result();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END