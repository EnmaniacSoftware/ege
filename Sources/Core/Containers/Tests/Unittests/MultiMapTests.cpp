#include "TestFramework/Interface/TestBase.h"
#include <EGEMultiMap.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef MultiMap<s32, s32> IntMap;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const s32 KElementCount            = 4;
static const s32 KExistingElementKey      = 1;
static const s32 KExistingElementValue    = 11;
static const s32 KNonExistingElementKey   = 100;
static const s32 KNonExistingElementValue = 1000;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class MultiMapTest : public TestBase
{
  protected:

    /*! Constructs a container with predefined data.
     *  @param  container Container to be filled in with data.
     *  @note Any old data within the container will be removed.
     */
    void create(IntMap& container) const;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void MultiMapTest::create(IntMap& container) const
{
  container.clear();

  container.insert(0, 10);
  container.insert(KExistingElementKey, KExistingElementValue);
  container.insert(2, 12);
  container.insert(3, 13);

  EXPECT_EQ(KElementCount, container.size());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(MultiMapTest, Construct)
{
  IntMap empty;
  EXPECT_EQ(0, empty.size());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(MultiMapTest, IsEmpty)
{
  IntMap container;
  EXPECT_TRUE(container.isEmpty());

  IntMap other;
  create(other);
  EXPECT_FALSE(other.isEmpty());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(MultiMapTest, Clear)
{
  IntMap container;
  EXPECT_TRUE(container.isEmpty());

  create(container);
  EXPECT_FALSE(container.isEmpty());

  // clear
  container.clear();
  EXPECT_TRUE(container.isEmpty());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(MultiMapTest, Size)
{
  IntMap container;
  EXPECT_EQ(0, container.size());

  // insert element
  container.insert(10, 10);
  EXPECT_EQ(1, container.size());

  // insert element
  container.insert(20, 10);
  EXPECT_EQ(2, container.size());

  // insert additional value to an existing key
  container.insert(20, 20);
  EXPECT_EQ(3, container.size());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(MultiMapTest, RemoveByValue)
{
  IntMap container;
  create(container);

  // remove unexisting data
  EXPECT_FALSE(container.removeByValue(KNonExistingElementValue));

  // remove existing data
  EXPECT_TRUE(container.removeByValue(KExistingElementValue));
  EXPECT_EQ(KElementCount - 1, container.size());

  // check content
  for (IntMap::ConstIterator it = container.begin(); it != container.end(); ++it)
  {
    EXPECT_NE(KExistingElementKey, it->first);
    EXPECT_NE(KExistingElementValue, it->second);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(MultiMapTest, RemoveByIterator)
{
  IntMap container;
  create(container);

  // remove existing element
  IntMap::Iterator next = ++container.begin();
  EXPECT_EQ(next, container.remove(container.begin()));
  EXPECT_EQ(KElementCount - 1, container.size());

  // check content
  IntMap reference;
  create(reference);

  IntMap::ConstIterator it = container.begin();
  IntMap::ConstIterator it2 = ++reference.begin();
  for (; (it != container.end()) && (it2 != reference.end()); ++it, ++it2)
  {
    EXPECT_EQ(*it2, *it);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(MultiMapTest, Values)
{
  IntMap container;
  create(container);

  List<s32> values;

  // retrieve values for a non-existing key;
  values = container.values(KNonExistingElementKey);
  EXPECT_EQ(0, values.length());

  // retrieve values for an existing key;
  values = container.values(KExistingElementKey);
  EXPECT_EQ(1, values.length());

  // add additional elements for an existing key
  container.insert(KExistingElementKey, KExistingElementValue + 1);

  // retrieve values for an existing key again
  values = container.values(KExistingElementKey);
  EXPECT_EQ(2, values.length());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
