#ifndef EGE_CORE_HASH_H
#define EGE_CORE_HASH_H

/** This class calculates 32-bit hash values from given input data. */

#include <EGETypes.h>
#include <EGEString.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Hash
{
  public:

    /* Calculates hash from string. */
    static u32 FromString(const String& string);
    /* Calculates hash data. */
    static u32 FromData(const u8* data, s32 size);
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_HASH_H
