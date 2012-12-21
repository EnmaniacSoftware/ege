#ifndef EGE_CORE_RANDOMGENERATOR_PRIVATE_H
#define EGE_CORE_RANDOMGENERATOR_PRIVATE_H

#include <EGE.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class RandomGeneratorPrivate
{
  public:

    RandomGeneratorPrivate();
   ~RandomGeneratorPrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /*! Returns randomly generated integer. */
    s32 rand();
    /*! Sets random seed. */
    void setSeed(u32 seed);
    /*! Returns randomly generated floating value from given interval. */
    float32 rand(float32 min, float32 max);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RANDOMGENERATOR_PRIVATE_H