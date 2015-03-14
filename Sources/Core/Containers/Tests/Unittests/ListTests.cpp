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
  EXPECT_EQ(list1.length(), list2.length());

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
  EXPECT_EQ(0, empty.length());

  IntList single(10);
  EXPECT_EQ(1, single.length());

  IntList other;
  createList(other);
  IntList list(other);
  EXPECT_EQ(other.length(), list.length());

  // check content
  checkContentEqual(other, list);

  createList(other);
  IntList list2(other, 2);
  EXPECT_EQ(2, list2.length());

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
TEST_F(ListTest, RemoveByIterator)
{
  IntList list;
  createList(list);

  // remove existing element
  IntList::Iterator second = ++list.begin();
  EXPECT_EQ(second, list.remove(list.begin()));
  EXPECT_EQ(KListElementCount - 1, list.length());

  // check content
  IntList reference;
  createList(reference);

  IntList::ConstIterator it = list.begin();
  IntList::ConstIterator it2 = ++reference.begin();
  for (; (it != list.end()) && (it2 != reference.end()); ++it, ++it2)
  {
    EXPECT_EQ(*it2, *it);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ListTest, RemoveByValue)
{
  IntList list;
  createList(list);

  // remove non-existing element
  list.remove(KNonExistingElementValue);
  EXPECT_EQ(KListElementCount, list.length());

  // remove existing element
  list.remove(KExistingElementValue);
  EXPECT_EQ(KListElementCount - 1, list.length());

  // check content
  IntList::ConstIterator it = list.begin();
  for (; it != list.end(); ++it)
  {
    EXPECT_NE(KExistingElementValue, *it);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ListTest, RemoveFirst)
{
  IntList list;
  IntList referenceList;

  createList(list);
  createList(referenceList);

  // remove one item after anohter
  s32 itemsRemoved = 0;
  IntList::Iterator it = referenceList.begin();
  while ( ! list.isEmpty())
  {
    EXPECT_EQ(*list.begin(), *it);
    list.removeFirst();
    itemsRemoved++;
    it++;
    EXPECT_EQ(KListElementCount - itemsRemoved, list.length()); 
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ListTest, RemoveLast)
{
  IntList list;
  IntList referenceList;

  createList(list);
  createList(referenceList);

  // remove one item after anohter
  s32 itemsRemoved = 0;
  while ( ! list.isEmpty())
  {
    list.removeLast();
    
    itemsRemoved++;
    EXPECT_EQ(KListElementCount - itemsRemoved, list.length()); 
  
    IntList::Iterator it1 = list.begin();
    IntList::Iterator it2 = referenceList.begin();

    for (; it1 != list.end(); ++it1, ++it2)
    {
      EXPECT_EQ(*it2, *it1);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ListTest, First)
{
  IntList list;
  const IntList constEmptyList;

  // check on empty list
  EXPECT_EQ(KNonExistingElementValue, list.first(KNonExistingElementValue));
  EXPECT_EQ(KNonExistingElementValue, constEmptyList.first(KNonExistingElementValue));

  // check on non-empty list
  list << KExistingElementValue;
  EXPECT_EQ(KExistingElementValue, list.first(KNonExistingElementValue));

  const IntList constList(list);
  EXPECT_EQ(KExistingElementValue, constList.first(KNonExistingElementValue));

  // modify returned value
  list.first() = 200;
  EXPECT_EQ(200, list.first());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ListTest, Last)
{
  IntList list;
  const IntList constEmptyList;

  // check on empty list
  EXPECT_EQ(KNonExistingElementValue, list.last(KNonExistingElementValue));
  EXPECT_EQ(KNonExistingElementValue, constEmptyList.last(KNonExistingElementValue));

  // check on non-empty list
  list << KExistingElementValue;
  EXPECT_EQ(KExistingElementValue, list.last(KNonExistingElementValue));

  const IntList constList(list);
  EXPECT_EQ(KExistingElementValue, constList.last(KNonExistingElementValue));

  // modify returned value
  list.last() = 200;
  EXPECT_EQ(200, list.last());
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
  EXPECT_EQ(0, list.length());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ListTest, Append)
{
  IntList list;

  // operators
  list << 10;
  EXPECT_EQ(1, list.length());
  EXPECT_EQ(10, list.first(KNonExistingElementValue));

  IntList other;
  createList(other);
  list << other;
  EXPECT_EQ(KListElementCount + 1, list.length());

  // check content
  IntList::ConstIterator it1 = list.begin();
  EXPECT_EQ(10, *it1);

  it1++;
  IntList::ConstIterator it2 = other.begin();
  for (; (it1 != list.end()) && (it2 != other.end()); ++it1, ++it2)
  {
    EXPECT_EQ(*it1, *it2);
  }

  // methods
  list.clear();
  list.append(11);
  EXPECT_EQ(1, list.length());
  EXPECT_EQ(11, list.first(KNonExistingElementValue));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ListTest, Prepend)
{
  const s32 KRandomValue = randomInteger();

  IntList list;
  IntList other;
  createList(other);

  // prepend to empty list
  list.prepend(KRandomValue);
  EXPECT_EQ(1, list.length());
  EXPECT_EQ(KRandomValue, list.first());

  // prepend to non-empty list
  createList(list);
  list.prepend(KRandomValue);
  EXPECT_EQ(KListElementCount + 1, list.length());

  // check content
  IntList::ConstIterator it1 = list.begin();
  EXPECT_EQ(KRandomValue, *it1);

  it1++;
  IntList::ConstIterator it2 = other.begin();
  for (; (it1 != list.end()) && (it2 != other.end()); ++it1, ++it2)
  {
    EXPECT_EQ(*it1, *it2);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ListTest, Insert)
{
  const s32 KRandomValue = randomInteger();

  IntList list;
  IntList other;

  // insert into empty list
  IntList::Iterator returnIt = list.insert(list.begin(), KRandomValue);
  EXPECT_EQ(1, list.length());
  EXPECT_EQ(KRandomValue, *returnIt);
  EXPECT_EQ(*list.begin(), *returnIt);

  list.clear();
  returnIt = list.insert(list.end(), KRandomValue);
  EXPECT_EQ(1, list.length());
  EXPECT_EQ(KRandomValue, *returnIt);
  EXPECT_EQ(*list.begin(), *returnIt);

  // insert (prepend) into non-empty list
  createList(list);
  createList(other);
  other.prepend(KRandomValue);
  returnIt = list.insert(list.begin(), KRandomValue);
  EXPECT_EQ(KListElementCount + 1, list.length());
  checkContentEqual(other, list);

  // insert (append) into non-empty list
  createList(list);
  createList(other);
  other.append(KRandomValue);
  returnIt = list.insert(list.end(), KRandomValue);
  EXPECT_EQ(KListElementCount + 1, list.length());
  checkContentEqual(other, list);

  // insert (in the middle) into non-empty list
  const s32 insertPos = (randomInteger() % (KListElementCount - 2)) + 1;

  createList(list);
  returnIt = list.begin();
  for (s32 i = 0; i < insertPos; ++i)
  {
    ++returnIt;
  }

  returnIt = list.insert(returnIt, KRandomValue);
  EXPECT_EQ(KListElementCount + 1, list.length());
  EXPECT_EQ(KRandomValue, *returnIt);

  // check entire content
  createList(other);
  
  IntList::ConstIterator it1 = list.begin();
  IntList::ConstIterator it2 = other.begin();
  for (s32 i = 0; it1 != list.end(); ++it1, ++i)
  {
    if (insertPos != i)
    {
      EXPECT_EQ(*it1, *it2);
      ++it2;
    }
    else
    {
      EXPECT_EQ(KRandomValue, *it1);
    }
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
TEST_F(ListTest, BeginReverse)
{
  const IntList constListEmpty;
  IntList listEmpty;

  IntList list;
  createList(list);
  IntList constList(list);

  // const interator
  EXPECT_EQ(constListEmpty.rend(), constListEmpty.rbegin());
  EXPECT_NE(constList.rend(), constList.rbegin());

  IntList::ConstReverseIterator cit = constList.rbegin();
  for (s32 i = KListElementCount - 1; 0 <= i; --i, ++cit)
  {
    if (i == KExistingElementIndex)
    {
      EXPECT_EQ(KExistingElementValue, *cit);
    }
    else
    {
      EXPECT_NE(KExistingElementValue, *cit);
    }
  }

  // non-const interator
  EXPECT_EQ(listEmpty.rend(), listEmpty.rbegin());
  EXPECT_NE(list.rend(), list.rbegin());

  IntList::ReverseIterator it = list.rbegin();
  for (s32 i = KListElementCount - 1; 0 <= i; --i, ++it)
  {
    if (i == KExistingElementIndex)
    {
      EXPECT_EQ(KExistingElementValue, *it);
    }
    else
    {
      EXPECT_NE(KExistingElementValue, *it);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(ListTest, EndReverse)
{
  const IntList constListEmpty;
  IntList listEmpty;

  IntList list;
  createList(list);
  IntList constList(list);

  // const interator
  EXPECT_EQ(constListEmpty.rend(), constListEmpty.rbegin());
  EXPECT_NE(constList.rend(), constList.rbegin());

  // non-const interator
  EXPECT_EQ(listEmpty.rend(), listEmpty.rbegin());
  EXPECT_NE(list.rend(), list.rbegin());

  // test iteration eqivalence
  IntList::ConstReverseIterator cit = constList.rbegin();
  IntList::ReverseIterator it = list.rbegin();

  for (; (cit != constList.rend()) && (it != list.rend()); ++cit, ++it)
  {
    EXPECT_EQ(*cit, *it);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
