#include "TestFramework/Interface/TestBase.h"
#include <EGEDynamicArray.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const s32 KExistingValue      = 10;
static const s32 KNonExistingValue   = 1000;
static const s32 KDefaultArraySize   = 7;
static const s32 KExistingValueIndex = 1;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class DynamicArrayTest : public TestBase
{
  protected:

    /*! Fills in given array with predefined data. */
    void createArray(IntArray& array) const;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void DynamicArrayTest::createArray(IntArray& array) const
{
  array.clear();
  
  array << 1;
  array << 10;
  array << 2;
  array << 20;
  array << 3;
  array << 30;
  array << 100;

  EXPECT_EQ(KDefaultArraySize, array.size());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(DynamicArrayTest, Construct)
{
  IntArray empty;
  EXPECT_EQ(0, empty.size());

  IntArray other;
  other << 1;
  other << 2;
  other << 3;

  IntArray another(other);
  EXPECT_EQ(other.size(), another.size());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(DynamicArrayTest, Length)
{
  IntArray array;
  EXPECT_EQ(0, array.size());

  IntArray array2(10);
  EXPECT_EQ(1, array2.size());

  IntArray array3;
  array3 << 1 << 2 << 3;
  EXPECT_EQ(3, array3.size());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(DynamicArrayTest, Clear)
{
  IntArray array;
  createArray(array);

  array.clear(); 
  EXPECT_EQ(0, array.size());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(DynamicArrayTest, IsEmpty)
{
  IntArray array;
  EXPECT_TRUE(array.isEmpty());

  IntArray array2(10);
  EXPECT_FALSE(array2.isEmpty());

  IntArray other;
  createArray(other);

  IntArray array3(other);
  EXPECT_FALSE(array3.isEmpty());

  array3.clear();
  EXPECT_TRUE(array3.isEmpty());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(DynamicArrayTest, Remove)
{
  IntArray array;
  createArray(array);

  // remove unexisting value
  array.remove(KNonExistingValue);
  EXPECT_EQ(KDefaultArraySize, array.size());

  // remove existing value
  array.remove(KExistingValue);
  EXPECT_EQ(KDefaultArraySize - 1, array.size());

  for (s32 i = 0; i < array.size(); ++i)
  {
    EXPECT_TRUE(KExistingValue != array.at(i, KExistingValue));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(DynamicArrayTest, RemoveAt)
{
  IntArray array;
  createArray(array);

  // remove value at unexisting index
  array.removeAt(-1);
  EXPECT_EQ(KDefaultArraySize, array.size());

  array.removeAt(KDefaultArraySize);
  EXPECT_EQ(KDefaultArraySize, array.size());

  // remove value at existing index
  array.removeAt(KExistingValueIndex);
  EXPECT_EQ(KDefaultArraySize - 1, array.size());

  for (s32 i = 0; i < array.size(); ++i)
  {
    EXPECT_TRUE(KExistingValue != array.at(i, KExistingValue));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(DynamicArrayTest, RemoveByIterator)
{
  IntArray array;
  createArray(array);

  // remove existing element
  s32 secondValue = array[1];
  EXPECT_EQ(secondValue, *array.remove(array.begin()));
  EXPECT_EQ(KDefaultArraySize - 1, array.size());

  // check content
  IntArray reference;
  createArray(reference);

  IntArray::ConstIterator it = array.begin();
  IntArray::ConstIterator it2 = ++reference.begin();
  for (; (it != array.end()) && (it2 != reference.end()); ++it, ++it2)
  {
    EXPECT_EQ(*it2, *it);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(DynamicArrayTest, RemoveLast)
{
  IntArray array;
  IntArray referenceArray;

  createArray(array);
  createArray(referenceArray);

  // remove one item after anohter
  s32 itemsRemoved = 0;
  while ( ! array.isEmpty())
  {
    array.removeLast();
    
    itemsRemoved++;
    EXPECT_EQ(KDefaultArraySize - itemsRemoved, array.size()); 
  
    IntArray::Iterator it1 = array.begin();
    IntArray::Iterator it2 = referenceArray.begin();

    for (; it1 != array.end(); ++it1, ++it2)
    {
      EXPECT_EQ(*it2, *it1);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(DynamicArrayTest, First)
{
  IntArray array;
  const IntArray constEmptyArray;

  // check on empty array
  EXPECT_EQ(KNonExistingValue, array.first(KNonExistingValue));
  EXPECT_EQ(KNonExistingValue, constEmptyArray.first(KNonExistingValue));

  // check on non-empty array
  array << KExistingValue;
  EXPECT_EQ(KExistingValue, array.first(KNonExistingValue));

  const IntArray constArray(array);
  EXPECT_EQ(KExistingValue, constArray.first(KNonExistingValue));

  // modify returned value
  array.first() = 200;
  EXPECT_EQ(200, array.first());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(DynamicArrayTest, Last)
{
  IntArray array;
  const IntArray constEmptyArray;

  // check on empty list
  EXPECT_EQ(KNonExistingValue, array.last(KNonExistingValue));
  EXPECT_EQ(KNonExistingValue, constEmptyArray.last(KNonExistingValue));

  // check on non-empty list
  array << KExistingValue;
  EXPECT_EQ(KExistingValue, array.last(KNonExistingValue));

  const IntArray constArray(array);
  EXPECT_EQ(KExistingValue, constArray.last(KNonExistingValue));

  // modify returned value
  array.last() = 200;
  EXPECT_EQ(200, array.last());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(DynamicArrayTest, Contains)
{
  IntArray array;
  createArray(array);

  EXPECT_TRUE(array.contains(KExistingValue));
  EXPECT_FALSE(array.contains(KNonExistingValue));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(DynamicArrayTest, Copy)
{
  IntArray array1;
  
  IntArray array2;
  createArray(array2);

  array1 << 1;
  EXPECT_EQ(1, array1.size());

  // make a copy
  array1.copy(array2);
  EXPECT_EQ(array2.size(), array1.size());
  for (s32 i = 0; i < array1.size(); ++i)
  {
    EXPECT_EQ(array2.at(i, -1), array1.at(i, -2));
  }

  // copy empty array
  IntArray empty;
  array1.copy(empty);
  EXPECT_EQ(0, array1.size());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(DynamicArrayTest, Append)
{
  IntArray array;
  
  // operators
  array << 100;
  EXPECT_EQ(1, array.size());
  EXPECT_EQ(100, array.at(0, 0));

  array << 101;
  EXPECT_EQ(2, array.size());
  EXPECT_EQ(101, array.at(1, 0));

  IntArray array2;
  createArray(array2);

  array << array2;
  EXPECT_EQ(2 + KDefaultArraySize, array.size());
  for (s32 i = 2; i < array.size(); ++i)
  {
    EXPECT_EQ(array2.at(i - 2, -1), array.at(i, -2));
  }

  // methods
  array.clear();
  array.append(11);
  EXPECT_EQ(1, array.size());
  EXPECT_EQ(11, array.at(0, KNonExistingValue));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(DynamicArrayTest, At)
{
  IntArray array;

  // retrieve non-existing element from empty array
  EXPECT_EQ(KNonExistingValue, array.at(0, KNonExistingValue));
  EXPECT_EQ(KNonExistingValue, array.at(-1, KNonExistingValue));

  createArray(array);

  // retrieve existing element
  EXPECT_EQ(KExistingValue, array.at(KExistingValueIndex, KNonExistingValue));
  
  // retrieve non-existing element from non-empty array
  EXPECT_EQ(KNonExistingValue, array.at(KDefaultArraySize, KNonExistingValue));
  EXPECT_EQ(KNonExistingValue, array.at(-1, KNonExistingValue));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(DynamicArrayTest, IndexOf)
{
  IntArray array;
  createArray(array);

  // check for non-existing element
  EXPECT_GE(0, array.indexOf(KNonExistingValue));

  // check for existing element
  EXPECT_EQ(KExistingValueIndex, array.indexOf(KExistingValue));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(DynamicArrayTest, ArrayOperators)
{
  IntArray array;
  createArray(array);

  IntArray constArray(array);

  // retrieve existing element
  EXPECT_EQ(KExistingValue, array[KExistingValueIndex]);
  EXPECT_EQ(KExistingValue, constArray[KExistingValueIndex]);
  
  // modify and verify
  array[KExistingValueIndex] = 22;
  EXPECT_EQ(22, array[KExistingValueIndex]);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(DynamicArrayTest, SetCapacity)
{
  IntArray array;
  
  array << 1;
  array << 2;
  array << 3;

  // change capacity to smaller
  array.setCapacity(2);

  // check content
  EXPECT_EQ(3, array.size());
  EXPECT_EQ(1, array[0]);
  EXPECT_EQ(2, array[1]);
  EXPECT_EQ(3, array[2]);

  // change capacity to the same size
  array.setCapacity(3);

  // check content
  EXPECT_EQ(3, array.size());
  EXPECT_EQ(1, array[0]);
  EXPECT_EQ(2, array[1]);
  EXPECT_EQ(3, array[2]);

  // change capacity to greater size
  array.setCapacity(10);

  // check content
  EXPECT_EQ(3, array.size());
  EXPECT_EQ(1, array[0]);
  EXPECT_EQ(2, array[1]);
  EXPECT_EQ(3, array[2]);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(DynamicArrayTest, SetLength)
{
  IntArray array;
  IntArray array2;
  
  // set some initial capacity
  array.setSize(3);
  EXPECT_EQ(3, array.size());

  // fill in with known data
  createArray(array);
  createArray(array2);

  // shrink
  const s32 KShrinkedLength = array.size() / 2;
  array.setSize(KShrinkedLength);

  // check content
  for (s32 i = 0; i < KShrinkedLength; ++i)
  {
    EXPECT_EQ(array2[i], array[i]);
  }

  // expand
  createArray(array);

  const s32 KExpandedLength = array.size() * 2;
  array.setSize(KExpandedLength);
  EXPECT_EQ(KExpandedLength, array.size());

  // check content
  for (s32 i = 0; i < array2.size(); ++i)
  {
    EXPECT_EQ(array2[i], array[i]);
  }

  // set the same size
  createArray(array);

  array.setCapacity(array.size());
  EXPECT_EQ(array2.size(), array.size());

  // check content
  for (s32 i = 0; i < array.size(); ++i)
  {
    EXPECT_EQ(array2[i], array[i]);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(DynamicArrayTest, Compare)
{
  IntArray array1;
  IntArray array2;
  
  createArray(array1);
  createArray(array2);

  // operators
  EXPECT_TRUE(array1 == array2);
  EXPECT_TRUE(array2 == array1);

  // change an element
  array1[0]++;

  EXPECT_FALSE(array1 == array2);
  EXPECT_FALSE(array2 == array1);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(DynamicArrayTest, Begin)
{
  const IntArray constArrayEmpty;
  IntArray arrayEmpty;

  IntArray array;
  createArray(array);
  IntArray constArray(array);

  // const interator
  EXPECT_EQ(constArrayEmpty.end(), constArrayEmpty.begin());
  EXPECT_NE(constArray.end(), constArray.begin());

  IntArray::ConstIterator cit = constArray.begin();
  for (s32 i = 0; i < KDefaultArraySize; ++i, ++cit)
  {
    if (i == KExistingValueIndex)
    {
      EXPECT_EQ(KExistingValue, *cit);
    }
  }

  // non-const interator
  EXPECT_EQ(arrayEmpty.end(), arrayEmpty.begin());
  EXPECT_NE(array.end(), array.begin());

  IntArray::Iterator it = array.begin();
  for (s32 i = 0; i < KDefaultArraySize; ++i, ++it)
  {
    if (i == KExistingValueIndex)
    {
      EXPECT_EQ(KExistingValue, *it);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(DynamicArrayTest, End)
{
  const IntArray constArrayEmpty;
  IntArray arrayEmpty;

  IntArray array;
  createArray(array);
  IntArray constArray(array);

  // const interator
  EXPECT_EQ(constArrayEmpty.end(), constArrayEmpty.begin());
  EXPECT_NE(constArray.end(), constArray.begin());

  // non-const interator
  EXPECT_EQ(arrayEmpty.end(), arrayEmpty.begin());
  EXPECT_NE(array.end(), array.begin());

  // test iteration eqivalence
  IntArray::ConstIterator cit = constArray.begin();
  IntArray::Iterator it = array.begin();

  for (; (cit != constArray.end()) && (it != array.end()); ++cit, ++it)
  {
    EXPECT_EQ(*cit, *it);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(DynamicArrayTest, BeginReverse)
{
  const IntArray constArrayEmpty;
  IntArray arrayEmpty;

  IntArray array;
  createArray(array);
  IntArray constArray(array);

  // const interator
  EXPECT_EQ(constArrayEmpty.rend(), constArrayEmpty.rbegin());
  EXPECT_NE(constArray.rend(), constArray.rbegin());

  IntArray::ConstReverseIterator cit = constArray.rbegin();
  for (s32 i = KDefaultArraySize - 1; 0 <= i; --i, ++cit)
  {
    if (i == KExistingValueIndex)
    {
      EXPECT_EQ(KExistingValue, *cit);
    }
    else
    {
      EXPECT_NE(KExistingValue, *cit);
    }
  }

  // non-const interator
  EXPECT_EQ(arrayEmpty.rend(), arrayEmpty.rbegin());
  EXPECT_NE(array.rend(), array.rbegin());

  IntArray::ReverseIterator it = array.rbegin();
  for (s32 i = KDefaultArraySize - 1; 0 <= i; --i, ++it)
  {
    if (i == KExistingValueIndex)
    {
      EXPECT_EQ(KExistingValue, *it);
    }
    else
    {
      EXPECT_NE(KExistingValue, *it);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(DynamicArrayTest, EndReverse)
{
  const IntArray constArrayEmpty;
  IntArray arrayEmpty;

  IntArray array;
  createArray(array);
  IntArray constArray(array);

  // const interator
  EXPECT_EQ(constArrayEmpty.rend(), constArrayEmpty.rbegin());
  EXPECT_NE(constArray.rend(), constArray.rbegin());

  // non-const interator
  EXPECT_EQ(arrayEmpty.rend(), arrayEmpty.rbegin());
  EXPECT_NE(array.rend(), array.rbegin());

  // test iteration eqivalence
  IntArray::ConstReverseIterator cit = constArray.rbegin();
  IntArray::ReverseIterator it = array.rbegin();

  for (; (cit != constArray.rend()) && (it != array.rend()); ++cit, ++it)
  {
    EXPECT_EQ(*cit, *it);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------