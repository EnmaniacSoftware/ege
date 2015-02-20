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

    /*! Checks content of the given lists for equality. 
     *  @param  list1 First list.
     *  @param  list2 Second list.
     */
    void checkContentEqual(const IntList& list1, const IntList& list2) const;
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

  EXPECT_EQ(KListElementCount, list.length());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ListTest::checkContentEqual(const IntList& list1, const IntList& list2) const
{
  EXPECT_EQ(list1.size(), list2.size());

  IntList::ConstIterator it1 = list1.begin();
  IntList::ConstIterator it2 = list2.begin();
  for (; (it1 != list1.end()) && (it2 != list2.end()); ++it1, ++it2)
  {
    EXPECT_EQ(*it1, *it2);
  }
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

  // check content
  checkContentEqual(other, list);

  createList(other);
  IntList list2(other, 2);
  EXPECT_EQ(2, list2.size());

  // check content
  IntList::ConstIterator it1 = list2.begin();
  IntList::ConstIterator it2 = other.begin();
  for (; (it1 != list2.end()) && (it2 != other.end()); ++it1, ++it2)
  {
    EXPECT_EQ(*it1, *it2);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ListTest, Length)
{
  IntList list;
  EXPECT_EQ(0, list.length());

  IntList list2(10);
  EXPECT_EQ(1, list2.length());

  IntList list3;
  list3 << 1 << 2 << 3;
  EXPECT_EQ(3, list3.length());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ListTest, Clear)
{
  IntList list;
  createList(list);

  list.clear(); 
  EXPECT_EQ(0, list.length());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ListTest, IsEmpty)
{
  IntList list;
  EXPECT_TRUE(list.isEmpty());

  IntList list2(10);
  EXPECT_FALSE(list2.isEmpty());

  IntList other;
  createList(other);

  IntList list3(other);
  EXPECT_FALSE(list3.isEmpty());

  list3.clear();
  EXPECT_TRUE(list3.isEmpty());
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

  // check content
  IntList::ConstIterator it = list.begin();
  for (; it != list.end(); ++it)
  {
    EXPECT_NE(KExistingElementValue, *it);
  }
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
  checkContentEqual(other, list);

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
  IntList::ConstIterator it1 = list.begin();
  EXPECT_EQ(10, *it1);

  it1++;
  IntList::ConstIterator it2 = other.begin();
  for (; (it1 != list.end()) && (it2 != other.end()); ++it1, ++it2)
  {
    EXPECT_EQ(*it1, *it2);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ListTest, Begin)
{
  const IntList constListEmpty;
  IntList listEmpty;

  IntList list;
  createList(list);
  IntList constList(list);

  // const interator
  EXPECT_EQ(constListEmpty.end(), constListEmpty.begin());
  EXPECT_NE(constList.end(), constList.begin());

  IntList::ConstIterator cit = constList.begin();
  for (s32 i = 0; i < KListElementCount; ++i, ++cit)
  {
    if (i == KExistingElementIndex)
    {
      EXPECT_EQ(KExistingElementValue, *cit);
    }
  }

  // non-const interator
  EXPECT_EQ(listEmpty.end(), listEmpty.begin());
  EXPECT_NE(list.end(), list.begin());

  IntList::Iterator it = list.begin();
  for (s32 i = 0; i < KListElementCount; ++i, ++it)
  {
    if (i == KExistingElementIndex)
    {
      EXPECT_EQ(KExistingElementValue, *it);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ListTest, End)
{
  const IntList constListEmpty;
  IntList listEmpty;

  IntList list;
  createList(list);
  IntList constList(list);

  // const interator
  EXPECT_EQ(constListEmpty.end(), constListEmpty.begin());
  EXPECT_NE(constList.end(), constList.begin());

  // non-const interator
  EXPECT_EQ(listEmpty.end(), listEmpty.begin());
  EXPECT_NE(list.end(), list.begin());

  // test iteration eqivalence
  IntList::ConstIterator cit = constList.begin();
  IntList::Iterator it = list.begin();

  for (; (cit != constList.end()) && (it != list.end()); ++cit, ++it)
  {
    EXPECT_EQ(*cit, *it);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------