#include "TestFramework/Interface/TestBase.h"
#include <EGEMap.h>
#include <list>

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
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(MapTest, IsEmpty)
{
  IntMap container;
  EXPECT_TRUE(container.isEmpty());

  IntMap other;
  create(other);
  EXPECT_FALSE(other.isEmpty());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(MapTest, Clear)
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
TEST_F(MapTest, Size)
{
  IntMap container;
  EXPECT_EQ(0, container.size());

  // insert element
  container[10] = 10;
  EXPECT_EQ(1, container.size());

  // insert element
  container[20] = 10;
  EXPECT_EQ(2, container.size());

  // replace existing element
  container[20] = 20;
  EXPECT_EQ(2, container.size());
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
  for (IntMap::ConstIterator it = container.begin(); it != container.end(); ++it)
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
  for (IntMap::ConstIterator it = container.begin(); it != container.end(); ++it)
  {
    EXPECT_NE(KExistingElementKey, it->first);
    EXPECT_NE(KExistingElementValue, it->second);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(MapTest, RemoveByIterator)
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
TEST_F(MapTest, Insert)
{
  IntMap container;

  // via operators
  container[KExistingElementKey] = KExistingElementValue;
  EXPECT_EQ(1, container.size());
  EXPECT_EQ(KExistingElementValue, container.value(KExistingElementKey, KNonExistingElementValue));

  // reinsert different value for the same key
  container[KExistingElementKey] = KNonExistingElementValue;
  EXPECT_EQ(1, container.size());
  EXPECT_EQ(KNonExistingElementValue, container[KExistingElementKey]);

  // via methods
  container.clear();
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

  // via operators

  // retrieve existing value
  EXPECT_EQ(KExistingElementValue, container[KExistingElementKey]);
  EXPECT_EQ(KElementCount, container.size());
  
  // via methods

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
TEST_F(MapTest, Append)
{
  IntMap container;
  IntMap container2;
  create(container);
  create(container2);

  // make sure a second container has some extra data and existing one a bit different
  container2[KExistingElementKey]    = KExistingElementValue + 1;
  container2[KNonExistingElementKey] = KNonExistingElementValue;

  // append without overrides
  container.append(container2, false);

  EXPECT_EQ(KElementCount + 1, container.size());
  EXPECT_EQ(KExistingElementValue, container[KExistingElementKey]);

  // reset containers
  create(container);
  create(container2);

  container2[KExistingElementKey]    = KExistingElementValue + 1;
  container2[KNonExistingElementKey] = KNonExistingElementValue;

  // append with overrides
  container.append(container2, true);

  EXPECT_EQ(KElementCount + 1, container.size());
  EXPECT_EQ(KExistingElementValue + 1, container[KExistingElementKey]);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(MapTest, BeginEndEmptyContainer)
{
  const IntMap constContainerEmpty;
  IntMap emptyContainer;

  // const interator
  IntMap::ConstIterator constIt1 = constContainerEmpty.end();
  IntMap::ConstIterator constIt2 = constContainerEmpty.begin();
  IntMap::ConstReverseIterator constReverseIt1 = constContainerEmpty.rend();
  IntMap::ConstReverseIterator constReverseIt2 = constContainerEmpty.rbegin();

  EXPECT_EQ(constIt1, constIt2);
  EXPECT_EQ(constReverseIt1, constReverseIt2);

  // non-const interator
  IntMap::Iterator it1 = emptyContainer.end();
  IntMap::Iterator it2 = emptyContainer.begin();
  IntMap::ReverseIterator reverseIt1 = emptyContainer.rend();
  IntMap::ReverseIterator reverseIt2 = emptyContainer.rbegin();

  EXPECT_EQ(it1, it2);
  EXPECT_EQ(reverseIt1, reverseIt2);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(MapTest, BeginEndNonEmptyContainer)
{
  IntMap container;
  create(container);

  // const interator
  IntMap::ConstIterator constIt1 = container.end();
  IntMap::ConstIterator constIt2 = container.begin();
  IntMap::ConstReverseIterator constReverseIt1 = container.rend();
  IntMap::ConstReverseIterator constReverseIt2 = container.rbegin();

  EXPECT_NE(constIt1, constIt2);
  EXPECT_NE(constReverseIt1, constReverseIt2);

  // non-const interator
  IntMap::Iterator it1 = container.end();
  IntMap::Iterator it2 = container.begin();
  IntMap::ReverseIterator reverseIt1 = container.rend();
  IntMap::ReverseIterator reverseIt2 = container.rbegin();
  
  EXPECT_NE(it1, it2);
  EXPECT_NE(reverseIt1, reverseIt2);

  // check content via non-const vs const
  IntMap::ConstIterator cit = container.begin();
  IntMap::Iterator it = container.begin();
  for (; (cit != container.end()) && (it != container.end()); ++cit, ++it)
  {      
    EXPECT_EQ(cit->first, it->first);
    EXPECT_EQ(cit->second, it->second);
  }

  IntMap::ConstReverseIterator rcit = container.rbegin();
  IntMap::ReverseIterator rit = container.rbegin();
  for (; (rcit != container.rend()) && (rit != container.rend()); ++rcit, ++rit)
  {      
    EXPECT_EQ(rcit->first, rit->first);
    EXPECT_EQ(rcit->second, rit->second);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(MapTest, Ordering)
{
  IntMap container;
  create(container);

  // const interator
  std::list<IntMap::ConstIterator> constIterators;
  for (IntMap::ConstIterator it = container.begin(); it != container.end(); ++it)
  {
    // add to begining of the list
    constIterators.push_front(it);
  }

  // verify against reverse iterators
  std::list<IntMap::ConstIterator>::iterator constListIt = constIterators.begin();
  for (IntMap::ConstReverseIterator rit = container.rbegin(); rit != container.rend(); ++rit, ++constListIt)
  {
    EXPECT_EQ((*constListIt)->first, rit->first);
    EXPECT_EQ((*constListIt)->second, rit->second);
  }

  // non-const interator
  std::list<IntMap::Iterator> iterators;
  for (IntMap::Iterator it = container.begin(); it != container.end(); ++it)
  {
    // add to begining of the list
    iterators.push_front(it);
  }

  // verify against reverse iterators
  std::list<IntMap::Iterator>::iterator listIt = iterators.begin();
  for (IntMap::ReverseIterator rit = container.rbegin(); rit != container.rend(); ++rit, ++listIt)
  {
    EXPECT_EQ((*listIt)->first, rit->first);
    EXPECT_EQ((*listIt)->second, rit->second);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

