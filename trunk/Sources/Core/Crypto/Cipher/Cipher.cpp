#include "Core/Crypto/Cipher/Cipher.h"
#include "Core/Crypto/Cipher/XOR/CipherXOR_p.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Cipher::Cipher(EGECipher::Algorithm algorithm, EGECipher::Direction direction, const PCipherKey& key) : m_p(NULL)
{
  switch (algorithm)
  {
    case EGECipher::ALGORITHM_XOR:

      m_p = ege_new CipherXORPrivate(this, direction, key);
      break;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Cipher::~Cipher()
{
  EGE_DELETE(m_p)
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Cipher::isValid() const
{
  return (NULL != m_p) && m_p->isValid();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult Cipher::addData(const char* data, s32 length)
{
  if (isValid())
  {
    return p_func()->addData(data, length);
  }

  return EGE_ERROR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult Cipher::addData(const PDataBuffer& data)
{
  if (isValid())
  {
    return p_func()->addData(data);
  }

  return EGE_ERROR;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Cipher::reset()
{
  if (isValid())
  {
    p_func()->reset();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PDataBuffer Cipher::result()
{
  PDataBuffer buffer;

  if (isValid())
  {
    //buffer = p_func()->result();
  }

  return buffer;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END