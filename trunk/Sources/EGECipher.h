#ifndef EGE_CIPHER_H
#define EGE_CIPHER_H

#include "Core/Platform.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

namespace EGECipher
{
  /*! Available encryption algorithms. */
  enum Algorithm
  {
    ALGORITHM_XOR = 0
  };

  /*! Available ciphering directions. */
  enum Direction
  {
    DIRECTION_ENCRYPT = 0,
    DIRECTION_DECRYPT
  };
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "Core/Crypto/Cipher/Cipher.h"
#include "Core/Crypto/Cipher/CipherKey.h"
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EGE_CIPHER_H