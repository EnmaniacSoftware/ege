#include "TestFramework/Interface/TestBase.h"
#include <EGEMap.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef Map<s32, s32> IntMap;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const s32 KElementCount            = 4;
static const s32 KExistingElementKey      = 1;
static const s32 KExistingElementValue    = 11;
static const s32 KNonExistingElementKey   = 100;
static const s32 KNonExistingElementValue = 1000;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class MapTest : public TestBase
{
  protected:

    /*! Constructs a container with predefined data.
     *  @param  container Container to be filled in with data.
     *  @note Any old data within the container will be removed.
     */
    void create(IntMap& container) const;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void MapTest::create(IntMap& container) const
{
  container.clear();

  container[0] = 10;
  container[KExistingElementKey] = KExistingElementValue;
  container[2] = 12;
  container[3] = 13;

  EXPECT_EQ(KElementCount, container.size());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(MapTest, Construct)
{
  IntMap empty;
  EXPECT_EQ(0, empty.size());

  //IntMap other;
  //createMap(other);
  //IntList list(other);
  //EXPECT_EQ(other.length(), list.length());

  //// check content
  //checkContentEqual(other, list);

  //createList(other);
  //IntList list2(other, 2);
  //EXPECT_EQ(2, list2.length());

  //// check content
  //IntList::ConstIterator it1 = list2.begin();
  //IntList::ConstIterator it2 = other.begin();
  //for (; (it1 != list2.end()) && (it2 != other.end()); ++it1, ++it2)
  //{
  //  EXPECT_EQ(*it1, *it2);
  //}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(MapTest, RemoveByKey)
{
  IntMap container;
  create(container);

  // remove unexisting data
  EXPECT_FALSE(container.removeByKey(KNonExistingElementKey));

  // remove existing data
  EXPECT_TRUE(container.removeByKey(KExistingElementKey));
  EXPECT_EQ(KElementCount - 1, container.size());

  // check content
  for (IntMap::const_iterator it = container.begin(); it != container.end(); ++it)
  {
    EXPECT_NE(KExistingElementKey, it->first);
    EXPECT_NE(KExistingElementValue, it->second);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(MapTest, RemoveByValue)
{
  IntMap container;
  create(container);

  // remove unexisting data
  EXPECT_FALSE(container.removeByValue(KNonExistingElementValue));

  // remove existing data
  EXPECT_TRUE(container.removeByValue(KExistingElementValue));
  EXPECT_EQ(KElementCount - 1, container.size());

  // check content
  for (IntMap::const_iterator it = container.begin(); it != container.end(); ++it)
  {
    EXPECT_NE(KExistingElementKey, it->first);
    EXPECT_NE(KExistingElementValue, it->second);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(MapTest, Insert)
{
  IntMap container;

  // insert into empty container
  container.insert(KExistingElementKey, KExistingElementValue);
  EXPECT_EQ(1, container.size());
  EXPECT_EQ(KExistingElementValue, container[KExistingElementKey]);

  // reinsert different value for the same key
  container.insert(KExistingElementKey, KNonExistingElementValue);
  EXPECT_EQ(1, container.size());
  EXPECT_EQ(KExistingElementValue, container[KExistingElementKey]);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(MapTest, Value)
{
  IntMap container;
  create(container);

  // retrieve non-existing value
  EXPECT_EQ(KNonExistingElementValue, container.value(KNonExistingElementKey, KNonExistingElementValue));

  // retrieve existing value
  EXPECT_EQ(KExistingElementValue, container.value(KExistingElementKey, KNonExistingElementValue));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(MapTest, Contains)
{
  IntMap container;
  create(container);

  // check non-existing value
  EXPECT_FALSE(container.contains(KNonExistingElementKey));

  // check existing value
  EXPECT_TRUE(container.contains(KExistingElementKey));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(MapTest, Merge)
{
  IntMap container;
  IntMap container2;
  create(container);
  create(container2);

  // make sure a second container has some extra data and existing one a bit different
  container2[KExistingElementKey]    = KExistingElementValue + 1;
  container2[KNonExistingElementKey] = KNonExistingElementValue;

  // merge without overrides
  container.merge(container2, false);

  EXPECT_EQ(KElementCount + 1, container.size());
  EXPECT_EQ(KExistingElementValue, container[KExistingElementKey]);

  // reset containers
  create(container);
  create(container2);

  container2[KExistingElementKey]    = KExistingElementValue + 1;
  container2[KNonExistingElementKey] = KNonExistingElementValue;

  // merge with overrides
  container.merge(container2, true);

  EXPECT_EQ(KElementCount + 1, container.size());
  EXPECT_EQ(KExistingElementValue + 1, container[KExistingElementKey]);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

