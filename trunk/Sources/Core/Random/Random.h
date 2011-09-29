#ifndef EGE_CORE_RANDOM_H
#define EGE_CORE_RANDOM_H

#include <EGE.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Random : public Object
{
  public:

    Random(u32 seed = 0);
   ~Random();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Returns randomly generated integer. */
    s32 operator() ();
    /* Returns randomly generated floating number from given interval. */
    float32 operator() (float32 min, float32 max);

    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /* Sets random seed. */
    void setSeed(u32 seed);

  private:

    EGE_DECLARE_PRIVATE_IMPLEMENTATION(Random);
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RANDOM_H