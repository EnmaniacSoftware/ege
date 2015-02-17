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
TEST_F(DynamicArrayTest, Last)
{
  IntArray array;

  // empty array
  EXPECT_EQ(KNonExistingValue, array.last(KNonExistingValue));

  // add an element to an array
  array << KExistingValue;
  EXPECT_EQ(KExistingValue, array.last(KNonExistingValue));

  // add another element to an array
  array << (KExistingValue + 1);
  EXPECT_EQ(KExistingValue + 1, array.last(KNonExistingValue));
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

