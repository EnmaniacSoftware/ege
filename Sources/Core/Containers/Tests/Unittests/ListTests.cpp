#include "TestFramework/Interface/TestBase.h"
#include <EGEList.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const s32 KListElementCount        = 7;
static const s32 KExistingElementIndex    = 1;
static const s32 KExistingElementValue    = 10;
static const s32 KNonExistingElementValue = 1000;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ListTest : public TestBase
{
  protected:

    /*! Constructs a list with predefined data.
     *  @param  list  List to be filled in with data.
     *  @note Any old data within the list will be removed.
     */
    void createList(IntList& list) const;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ListTest::createList(IntList& list) const
{
  list.clear();

  list << 1;
  list << 10;
  list << 2;
  list << 20;
  list << 3;
  list << 30;
  list << 100;

  EXPECT_EQ(KListElementCount, list.size());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ListTest, Construct)
{
  IntList empty;
  EXPECT_EQ(0, empty.size());

  IntList single(10);
  EXPECT_EQ(1, single.size());

  IntList other;
  createList(other);
  IntList list(other);
  EXPECT_EQ(other.size(), list.size());

  // TAGE - iterators needs to be implemented
//  for (s32 i = 0; i < other.size(); ++i)
//  {
//    EXPECT_EQ(other.at(i), list.at(i));
//  }

  createList(other);
  IntList list2(other, 2);
  EXPECT_EQ(2, list2.size());

  // TAGE - iterators needs to be implemented
  // Test elements
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ListTest, RemoveAt)
{
  IntList list;
  createList(list);

  // remove non-existing element
  list.removeAt(-1);
  EXPECT_EQ(KListElementCount, list.size());

  list.removeAt(KListElementCount);
  EXPECT_EQ(KListElementCount, list.size());

  // remove existing element
  list.removeAt(KExistingElementIndex);
  EXPECT_EQ(KListElementCount - 1, list.size());

  // TAGE - check content
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ListTest, First)
{
  IntList list;

  // check on empty list
  EXPECT_EQ(KNonExistingElementValue, list.first(KNonExistingElementValue));

  // check on non-empty list
  list << KExistingElementValue;
  EXPECT_EQ(KExistingElementValue, list.first(KNonExistingElementValue));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ListTest, Last)
{
  IntList list;

  // check on empty list
  EXPECT_EQ(KNonExistingElementValue, list.last(KNonExistingElementValue));

  // check on non-empty list
  list << KExistingElementValue;
  EXPECT_EQ(KExistingElementValue, list.last(KNonExistingElementValue));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ListTest, Contains)
{
  IntList list;
  createList(list);

  EXPECT_TRUE(list.contains(KExistingElementValue));
  EXPECT_FALSE(list.contains(KNonExistingElementValue));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ListTest, Copy)
{
  IntList list;

  IntList other;
  createList(other);

  list.copy(other);
  EXPECT_EQ(other.size(), list.size());
  // TAGE - check content

  list.copy(IntList());
  EXPECT_EQ(0, list.size());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ListTest, Append)
{
  IntList list;

  // operators
  list << 10;
  EXPECT_EQ(1, list.size());
  EXPECT_EQ(10, list.first(KNonExistingElementValue));

  IntList other;
  createList(other);
  list << other;
  EXPECT_EQ(KListElementCount + 1, list.size());

  // TAGE - check content
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
