#include "Core/Crypto/CryptographicHash.h"
#include "Core/Crypto/CryptographicHash_p.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
CryptographicHash::CryptographicHash(EGECryptographicHash::Algorithm algorithm) : m_p(NULL)
{
  // create private implementation
  m_p = ege_new CryptographicHashPrivate(algorithm);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
CryptographicHash::~CryptographicHash()
{
  EGE_DELETE(m_p)
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CryptographicHash::isValid() const
{
  return (NULL != m_p);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult CryptographicHash::addData(const char* data, s32 length)
{
  if (isValid())
  {
    return p_func()->addData(data, length);
  }

  return EGE_ERROR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult CryptographicHash::addData(const PDataBuffer& data)
{
  if (isValid())
  {
    return p_func()->addData(data);
  }

  return EGE_ERROR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void CryptographicHash::reset()
{
  if (isValid())
  {
    p_func()->reset();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PDataBuffer CryptographicHash::result()
{
  PDataBuffer buffer;

  if (isValid())
  {
    buffer = p_func()->result();
  }

  return buffer;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END