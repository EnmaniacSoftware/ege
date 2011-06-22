#ifndef EGE_CORE_RANDOM_PRIVATE_H
#define EGE_CORE_RANDOM_PRIVATE_H

#include <EGE.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Random;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class RandomPrivate
{
  public:

    RandomPrivate(Random* base);
   ~RandomPrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Returns randomly generated integer. */
    s32 rand();
    /* Sets random seed. */
    void setSeed(u32 seed);

  private:

    /*! Pointer to base object. */
    Random* m_base;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RANDOM_PRIVATE_H