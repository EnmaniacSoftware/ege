#include "Core/RandomGenerator/RandomGenerator.h"

#ifdef EGE_RANDOM_C
#include "Core/RandomGenerator/C/RandomGeneratorC_p.h"
#endif // EGE_RANDOM_C

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(RandomGenerator)
EGE_DEFINE_DELETE_OPERATORS(RandomGenerator)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RandomGenerator::RandomGenerator(u32 seed) : Object(NULL)
{
    m_p = ege_new RandomGeneratorPrivate(this);
  if (m_p && (0 < seed))
  {
    setSeed(seed);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RandomGenerator::~RandomGenerator()
{
  EGE_DELETE(m_p);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool RandomGenerator::isValid() const
{
  return NULL != m_p;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns randomly generated integer. */
s32 RandomGenerator::operator() ()
{
  if (isValid())
  {
    return p_func()->rand();
  }

  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets random seed. */
void RandomGenerator::setSeed(u32 seed)
{
  if (isValid())
  {
    p_func()->setSeed(seed);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns randomly generated floating number from given interval. */
float32 RandomGenerator::operator() (float32 min, float32 max)
{
  if (isValid())
  {
    return p_func()->rand(min, max);
  }

  return 0.0f;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
