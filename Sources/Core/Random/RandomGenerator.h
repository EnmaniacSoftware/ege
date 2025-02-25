#ifndef EGE_CORE_RANDOMGENERATOR_H
#define EGE_CORE_RANDOMGENERATOR_H

#include "EGE.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class RandomGenerator : public Object
{
  public:

    RandomGenerator(u32 seed = 0);
   ~RandomGenerator();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /*! Returns randomly generated integer. */
    s32 operator() ();
    /*! Returns randomly generated floating number from given interval. */
    float32 operator() (float32 min, float32 max);

    /*! Returns TRUE if object is valid. */
    bool isValid() const;
    /*! Sets random seed. */
    void setSeed(u32 seed);

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(RandomGenerator);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // #define EGE_CORE_RANDOMGENERATOR_H
