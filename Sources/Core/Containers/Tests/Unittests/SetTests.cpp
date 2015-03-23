#include "TestFramework/Interface/TestBase.h"
#include <EGETree.h>
#include <list>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef Tree<s32> IntSet;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const s32 KElementCount            = 4;
static const s32 KExistingElementValue    = 11;
static const s32 KNonExistingElementValue = 1000;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class SetTest : public TestBase
{
  protected:

    /*! Constructs a container with predefined data.
     *  @param  container Container to be filled in with data.
     *  @note Any old data within the container will be removed.
     */
    void create(IntSet& container) const;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void SetTest::create(IntSet& container) const
{
  container.clear();

  container.insert(10);
  container.insert(KExistingElementValue);
  container.insert(12);
  container.insert(13);

  EXPECT_EQ(KElementCount, container.size());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(SetTest, Construct)
{
  IntSet empty;
  EXPECT_EQ(0, empty.size());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(SetTest, IsEmpty)
{
  IntSet container;
  EXPECT_TRUE(container.isEmpty());

  IntSet other;
  create(other);
  EXPECT_FALSE(other.isEmpty());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(SetTest, Clear)
{
  IntSet container;
  EXPECT_TRUE(container.isEmpty());

  create(container);
  EXPECT_FALSE(container.isEmpty());

  // clear
  container.clear();
  EXPECT_TRUE(container.isEmpty());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(SetTest, Size)
{
  IntSet container;
  EXPECT_EQ(0, container.size());

  // insert element
  container.insert(1);
  EXPECT_EQ(1, container.size());

  // insert element
  container.insert(2);
  EXPECT_EQ(2, container.size());

  // try to insert existing element
  container.insert(2);
  EXPECT_EQ(2, container.size());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(SetTest, RemoveByValue)
{
  IntSet container;
  create(container);

  // remove unexisting data
  container.remove(KNonExistingElementValue);
  EXPECT_EQ(KElementCount, container.size());

  // remove existing data
  container.remove(KExistingElementValue);
  EXPECT_EQ(KElementCount - 1, container.size());

  // check content
  for (IntSet::ConstIterator it = container.begin(); it != container.end(); ++it)
  {
    EXPECT_NE(KExistingElementValue, *it);
  }
}
////--------------------------------------------------------------------------------------------------------------------------------------------------------------
//TEST_F(SetTest, RemoveByIterator)
//{
//  IntSet container;
//  create(container);
//
//  // remove existing element
//  IntSet::Iterator next = ++container.begin();
//  EXPECT_EQ(next, container.remove(container.begin()));
//  EXPECT_EQ(KElementCount - 1, container.size());
//
//  // check content
//  IntSet reference;
//  create(reference);
//
//  IntSet::ConstIterator it = container.begin();
//  IntSet::ConstIterator it2 = ++reference.begin();
//  for (; (it != container.end()) && (it2 != reference.end()); ++it, ++it2)
//  {
//    EXPECT_EQ(*it2, *it);
//  }
//}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(SetTest, Insert)
{
  IntSet container;

  // via operators

  // via methods
  container.insert(KExistingElementValue);
  EXPECT_EQ(1, container.size());

  // insert existing value once more
  container.insert(KExistingElementValue);
  EXPECT_EQ(1, container.size());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(SetTest, Contains)
{
  IntSet container;
  create(container);

  // check non-existing value
  EXPECT_FALSE(container.contains(KNonExistingElementValue));

  // check existing value
  EXPECT_TRUE(container.contains(KExistingElementValue));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(SetTest, BeginEndEmptyContainer)
{
  const IntSet constContainerEmpty;
  IntSet emptyContainer;

  // const interator
  IntSet::ConstIterator constIt1 = constContainerEmpty.end();
  IntSet::ConstIterator constIt2 = constContainerEmpty.begin();
  IntSet::ConstReverseIterator constReverseIt1 = constContainerEmpty.rend();
  IntSet::ConstReverseIterator constReverseIt2 = constContainerEmpty.rbegin();

  EXPECT_EQ(constIt1, constIt2);
  EXPECT_EQ(constReverseIt1, constReverseIt2);

  // non-const interator
  IntSet::Iterator it1 = emptyContainer.end();
  IntSet::Iterator it2 = emptyContainer.begin();
  IntSet::ReverseIterator reverseIt1 = emptyContainer.rend();
  IntSet::ReverseIterator reverseIt2 = emptyContainer.rbegin();

  EXPECT_EQ(it1, it2);
  EXPECT_EQ(reverseIt1, reverseIt2);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(SetTest, BeginEndNonEmptyContainer)
{
  IntSet container;
  create(container);

  // const interator
  IntSet::ConstIterator constIt1 = container.end();
  IntSet::ConstIterator constIt2 = container.begin();
  IntSet::ConstReverseIterator constReverseIt1 = container.rend();
  IntSet::ConstReverseIterator constReverseIt2 = container.rbegin();

  EXPECT_NE(constIt1, constIt2);
  EXPECT_NE(constReverseIt1, constReverseIt2);
 
  // non-const interator
  IntSet::Iterator it1 = container.end();
  IntSet::Iterator it2 = container.begin();
  IntSet::ReverseIterator reverseIt1 = container.rend();
  IntSet::ReverseIterator reverseIt2 = container.rbegin();
  
  EXPECT_NE(it1, it2);
  EXPECT_NE(reverseIt1, reverseIt2);

  // check content via non-const vs const
  IntSet::ConstIterator cit = container.begin();
  IntSet::Iterator it = container.begin();
  for (; (cit != container.end()) && (it != container.end()); ++cit, ++it)
  {      
    EXPECT_EQ(*cit, *it);
  }

  IntSet::ConstReverseIterator rcit = container.rbegin();
  IntSet::ReverseIterator rit = container.rbegin();
  for (; (rcit != container.rend()) && (rit != container.rend()); ++rcit, ++rit)
  {      
    EXPECT_EQ(*rcit, *rit);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(SetTest, Ordering)
{
  IntSet container;
  create(container);

  // const interator
  std::list<IntSet::ConstIterator> constIterators;
  for (IntSet::ConstIterator it = container.begin(); it != container.end(); ++it)
  {
    // add to begining of the list
    constIterators.push_front(it);
  }

  // verify against reverse iterators
  std::list<IntSet::ConstIterator>::iterator constListIt = constIterators.begin();
  for (IntSet::ConstReverseIterator rit = container.rbegin(); rit != container.rend(); ++rit, ++constListIt)
  {
    EXPECT_EQ(**constListIt, *rit);
  }

  // non-const interator
  std::list<IntSet::Iterator> iterators;
  for (IntSet::Iterator it = container.begin(); it != container.end(); ++it)
  {
    // add to begining of the list
    iterators.push_front(it);
  }

  // verify against reverse iterators
  std::list<IntSet::Iterator>::iterator listIt = iterators.begin();
  for (IntSet::ReverseIterator rit = container.rbegin(); rit != container.rend(); ++rit, ++listIt)
  {
    EXPECT_EQ(**listIt, *rit);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

