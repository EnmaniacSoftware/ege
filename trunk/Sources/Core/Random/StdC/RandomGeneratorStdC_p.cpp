#include "Core/Random/Random.h"
#include "Core/Random/C/RandomC_p.h"
#include <time.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(RandomGeneratorPrivate)
EGE_DEFINE_DELETE_OPERATORS(RandomGeneratorPrivate)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RandomGeneratorPrivate::RandomGeneratorPrivate()
{
  setSeed((u32) time(NULL));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RandomGeneratorPrivate::~RandomGeneratorPrivate()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets random seed. */
void RandomGeneratorPrivate::setSeed(u32 seed)
{
  srand(seed);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns randomly generated integer. */
s32 RandomGeneratorPrivate::rand()
{
  return ::rand();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns randomly generated floating value from given interval. */
float32 RandomGeneratorPrivate::rand(float32 min, float32 max)
{
  // generate value from [0-1]
  float32 value = static_cast<float32>(rand()) / RAND_MAX;

  // map it to [min, max]
  return min + value * (max - min);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
