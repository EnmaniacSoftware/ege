#include "TestFramework/Interface/TestBase.h"
#include <EGEMemory.h>
#include <EGEString.h>
#include <EGEStringArray.h>
#include <string>
#include <algorithm>
#include <cctype>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const char* KReferenceCString          = "This is C String";
static const std::string KReferenceStdString  = "This is STD String";
static const std::string KReferenceStdString2 = "This is another STD String";
static const std::string KReferenceStdString3 = "That is yet another STD String which is cool and it is really needed as is.";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class StringTest : public TestBase
{
  protected:

    static void SetUpTestCase();
    static void TearDownTestCase();

  protected:

    virtual void SetUp();
    virtual void TearDown();

  protected:
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void StringTest::SetUpTestCase()
{
  EXPECT_TRUE(MemoryManager::Initialize());
  }
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void StringTest::TearDownTestCase()
{
  MemoryManager::Deinitialize();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void StringTest::SetUp()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void StringTest::TearDown()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(StringTest, Construct)
{
  String empty;
  EXPECT_EQ(0, empty.length());

  String empty2(NULL);
  EXPECT_EQ(0, empty2.length());

  String empty3(NULL, 10);
  EXPECT_EQ(0, empty3.length());

  String fromConstChar(KReferenceCString);
  EXPECT_STREQ(KReferenceCString, fromConstChar.toAscii());

  String fromConstCharShort(KReferenceCString, 5);
  for (s32 i = 0; i < 5; ++i)
  {
    EXPECT_EQ(KReferenceCString[i], fromConstCharShort.toAscii()[i]);
  }  

  String fromStdString(KReferenceStdString);
  EXPECT_STREQ(KReferenceStdString.c_str(), fromStdString.toAscii());

  String fromAnotherString(fromConstChar);
  EXPECT_STREQ(KReferenceCString, fromAnotherString.toAscii());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(StringTest, Assignment)
{
  String string;

  // operators
  string = KReferenceCString;
  EXPECT_STREQ(KReferenceCString, string.toAscii());

  string = KReferenceStdString;
  EXPECT_STREQ(KReferenceStdString.c_str(), string.toAscii());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(StringTest, Equality)
{
  String string1(KReferenceCString);
  String string2(string1);

  // operators
  EXPECT_TRUE(string1 == KReferenceCString);
  EXPECT_TRUE(KReferenceCString == string1);

  EXPECT_TRUE(string1 == string2);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(StringTest, Inequality)
{
  String string1(KReferenceCString);
  String string2(KReferenceStdString);

  // operators
  EXPECT_FALSE(string1 != KReferenceCString);
  EXPECT_FALSE(KReferenceCString != string1);
  
  EXPECT_TRUE(string1 != string2);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(StringTest, LessThen)
{
  String string1(KReferenceCString);
  String string2(KReferenceStdString);

  const int referenceValue = strcmp(string1.toAscii(), string2.toAscii());

  EXPECT_EQ(referenceValue < 0 ? true : false, string1 < string2);
  EXPECT_EQ(referenceValue < 0 ? false : true, string2 < string1);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(StringTest, Append)
{
  String string1(KReferenceStdString);
  String string2(KReferenceStdString2);

  const char KCharacter = 'a';

  // operators +=
  String strings;
  strings += string1;
  EXPECT_STREQ(KReferenceStdString.c_str(), strings.toAscii());

  strings += string2;
  EXPECT_STREQ((KReferenceStdString + KReferenceStdString2).c_str(), strings.toAscii());

  strings += KCharacter;
  EXPECT_STREQ((KReferenceStdString + KReferenceStdString2 + KCharacter).c_str(), strings.toAscii());

  strings += KReferenceCString;
  EXPECT_STREQ((KReferenceStdString + KReferenceStdString2 + KCharacter + KReferenceCString).c_str(), strings.toAscii());

  // operators +
  strings = string1 + string2;
  EXPECT_STREQ((KReferenceStdString + KReferenceStdString2).c_str(), strings.toAscii());

  strings = string1 + KReferenceCString;
  EXPECT_STREQ((KReferenceStdString + KReferenceCString).c_str(), strings.toAscii());

  strings = KReferenceCString + string1;
  EXPECT_STREQ((KReferenceCString + KReferenceStdString).c_str(), strings.toAscii());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(StringTest, Length)
{
  String string;
  EXPECT_EQ(0, string.length());

  String string2("");
  EXPECT_EQ(0, string2.length());

  String string3(KReferenceStdString);
  EXPECT_EQ(static_cast<s32>(KReferenceStdString.size()), string3.length());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(StringTest, Clear)
{
  String string(KReferenceStdString);
  EXPECT_EQ(static_cast<s32>(KReferenceStdString.size()), string.length());

  string.clear(); 
  EXPECT_EQ(0, string.length());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(StringTest, IsEmpty)
{
  String string;
  EXPECT_TRUE(string.isEmpty());

  String string2("");
  EXPECT_TRUE(string2.isEmpty());

  String string3(KReferenceStdString);
  EXPECT_FALSE(string3.isEmpty());

  string3 = "";
  EXPECT_TRUE(string3.isEmpty());

  String string4(KReferenceStdString);
  EXPECT_FALSE(string4.isEmpty());

  string4.clear();
  EXPECT_TRUE(string4.isEmpty());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(StringTest, IndexOf)
{
  const char* KSubString            = "ST";
  const char* KNonExistingSubString = "blah";
  const s32 KSubStringPosition = static_cast<s32>(KReferenceStdString.find(KSubString, 0));
  EXPECT_TRUE(0 <= KSubStringPosition);

  // find existing substring starting from the begining
  String string(KReferenceStdString);
  EXPECT_EQ(KSubStringPosition, string.indexOf(KSubString, 0));

  // find existing substring starting at the exact position where it begins
  EXPECT_EQ(KSubStringPosition, string.indexOf(KSubString, KSubStringPosition));

  // find existing substring but starting after it
  EXPECT_GT(0, string.indexOf(KSubString, KSubStringPosition + 1));

  // find existing substring but starting after the whole string
  EXPECT_GT(0, string.indexOf(KSubString, string.length()));

  // find existing substring but starting before the whole string
  EXPECT_GT(0, string.indexOf(KSubString, -2));

  // find non-existing string
  EXPECT_GT(0, string.indexOf(KNonExistingSubString, 0));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(StringTest, LastIndexOf)
{
  const char* KSubString            = "is";
  const char* KNonExistingSubString = "blah";
  const s32 KSubStringPosition = static_cast<s32>(KReferenceStdString3.rfind(KSubString));

  // find existing substring starting from the begining
  String string(KReferenceStdString3);
  EXPECT_EQ(KSubStringPosition, string.lastIndexOf(KSubString));

//  // find existing substring starting at the exact position where it begins
//  EXPECT_EQ(KSubStringPosition, string.indexOf(KSubString, KSubStringPosition));

//  // find existing substring but starting after it
//  EXPECT_LT(0, string.indexOf(KSubString, KSubStringPosition + 1));

//  // find existing substring but starting after the whole string
//  EXPECT_LT(0, string.indexOf(KSubString, string.length()));

//  // find existing substring but starting before the whole string
//  EXPECT_LT(0, string.indexOf(KSubString, -2));

//  // find non-existing string
//  EXPECT_LT(0, string.indexOf(KNonExistingSubString, 0));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(StringTest, EndsWith)
{
  const char* KSubString = "String";
  const char* KNonExistingSubString = "string";
  const char* KNonExistingSubString2 = "String.";
  const char* KShortString = "tring";

  String string(KReferenceStdString);
  EXPECT_TRUE(string.endsWith(KSubString));
  EXPECT_FALSE(string.endsWith(KNonExistingSubString));
  EXPECT_FALSE(string.endsWith(KNonExistingSubString2));

  string = KShortString;
  EXPECT_FALSE(string.endsWith(KSubString));
  EXPECT_FALSE(string.endsWith(KNonExistingSubString));
  EXPECT_FALSE(string.endsWith(KNonExistingSubString2));
  EXPECT_TRUE(string.endsWith(KShortString));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(StringTest, StartsWith)
{
  const char* KSubString = "This";
  const char* KNonExistingSubString = "this";
  const char* KNonExistingSubString2 = "his.";

  String string(KReferenceStdString);
  EXPECT_TRUE(string.startsWith(KSubString));
  EXPECT_FALSE(string.startsWith(KNonExistingSubString));
  EXPECT_FALSE(string.startsWith(KNonExistingSubString2));
  EXPECT_FALSE(string.startsWith(KReferenceStdString + "."));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(StringTest, At)
{
  String string(KReferenceStdString);
  for (s32 i = 0; i < static_cast<s32>(KReferenceStdString.size()); ++i)
  {
    EXPECT_EQ(string.at(i), KReferenceStdString[i]);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(StringTest, SubString)
{
  String string(KReferenceStdString2);

  EXPECT_TRUE(string.subString(0, 4) == "This");
  EXPECT_TRUE(string.subString(5, 2) == "is");
  EXPECT_TRUE(string.subString(8, -1) == "another STD String");
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(StringTest, Format)
{
  String string;

  // %c
  string.format("This is an %c.", 'a');
  EXPECT_STREQ("This is an a.", string.toAscii());

  // %s
  string.format("This is an %s.", "Alabama");
  EXPECT_STREQ("This is an Alabama.", string.toAscii());

  // %d
  string.format("This is %d.", 125);
  EXPECT_STREQ("This is 125.", string.toAscii());
  string.format("This is %d.", -234);
  EXPECT_STREQ("This is -234.", string.toAscii());
  string.format("This is %d.", -2147483647 - 1);
  EXPECT_STREQ("This is -2147483648.", string.toAscii());
  string.format("This is %d.", 2147483647);
  EXPECT_STREQ("This is 2147483647.", string.toAscii());

  // %f
  // TAGE - add
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(StringTest, ToLower)
{
  String string(KReferenceStdString2);

  // create reference lower-case string
  std::string reference = KReferenceStdString2;
  std::transform(reference.begin(), reference.end(), reference.begin(), (int(*)(int)) std::tolower);

  EXPECT_STREQ(reference.c_str(), string.toLower().toAscii());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(StringTest, ToUpper)
{
  String string(KReferenceStdString2);

  // create reference upper-case string
  std::string reference = KReferenceStdString2;
  std::transform(reference.begin(), reference.end(), reference.begin(), (int(*)(int)) std::toupper);

  EXPECT_STREQ(reference.c_str(), string.toUpper().toAscii());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(StringTest, ToAscii)
{
  String string(KReferenceCString);

  EXPECT_STREQ(KReferenceCString, string.toAscii());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(StringTest, ToInt)
{
  bool error = false;

  // empty string conversion
  String string;
  string.toInt(&error);
  EXPECT_TRUE(error);

  // invalid values
  error = false;
  string = "Dummy Text";
  string.toInt(&error);
  EXPECT_TRUE(error);

  error = false;
  string = "@123";
  string.toInt(&error);
  EXPECT_TRUE(error);

  error = false;
  string = "123$";
  string.toInt(&error);
  EXPECT_TRUE(error);

  error = false;
  string = "123.90";
  string.toInt(&error);
  EXPECT_TRUE(error);

  // valid values
  error = false;
  string = "3890";
  EXPECT_EQ(3890, string.toInt(&error));
  EXPECT_FALSE(error);

  string = "+4590";
  EXPECT_EQ(4590, string.toInt(&error));
  EXPECT_FALSE(error);

  string = "-2147483648";
  EXPECT_EQ(-2147483647 - 1, string.toInt(&error));
  EXPECT_FALSE(error);

  string = "2147483647";
  EXPECT_EQ(2147483647, string.toInt(&error));
  EXPECT_FALSE(error);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(StringTest, ToUInt)
{
  bool error = false;

  // empty string conversion
  String string;
  string.toUInt(&error);
  EXPECT_TRUE(error);

  // invalid values
  error = false;
  string = "Dummy Text";
  string.toUInt(&error);
  EXPECT_TRUE(error);

  error = false;
  string = "@123";
  string.toUInt(&error);
  EXPECT_TRUE(error);

  error = false;
  string = "123$";
  string.toUInt(&error);
  EXPECT_TRUE(error);

  error = false;
  string = "123.90";
  string.toUInt(&error);
  EXPECT_TRUE(error);

  // valid values
  error = false;
  string = "3890";
  EXPECT_EQ(3890, string.toUInt(&error));
  EXPECT_FALSE(error);

  string = "+4590";
  EXPECT_EQ(4590, string.toUInt(&error));
  EXPECT_FALSE(error);

  string = "-100";
  EXPECT_EQ(static_cast<u32>(-100), string.toUInt(&error));
  EXPECT_FALSE(error);

  string = "4294967295";
  EXPECT_EQ(4294967295, string.toUInt(&error));
  EXPECT_FALSE(error);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(StringTest, ToInt64)
{
  bool error = false;

  // empty string conversion
  String string;
  error = false;
  string.toInt64(&error);
  EXPECT_TRUE(error);

  // invalid values
  error = false;
  string = "Dummy Text";
  string.toInt64(&error);
  EXPECT_TRUE(error);

  error = false;
  string = "@123";
  string.toInt64(&error);
  EXPECT_TRUE(error);

  error = false;
  string = "123$";
  string.toInt64(&error);
  EXPECT_TRUE(error);

  error = false;
  string = "123.90";
  string.toInt64(&error);
  EXPECT_TRUE(error);

  // valid values
  error = false;
  string = "3890";
  EXPECT_EQ(3890, string.toInt64(&error));
  EXPECT_FALSE(error);

  string = "+4590";
  EXPECT_EQ(4590, string.toInt64(&error));
  EXPECT_FALSE(error);

  string = "-9223372036854775808";
  EXPECT_EQ(-9223372036854775808LL, string.toInt64(&error));
  EXPECT_FALSE(error);

  string = "9223372036854775807";
  EXPECT_EQ(9223372036854775807LL, string.toInt64(&error));
  EXPECT_FALSE(error);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(StringTest, ToUInt64)
{
  bool error = false;

  // empty string conversion
  String string;
  error = false;
  string.toUInt64(&error);
  EXPECT_TRUE(error);

  // invalid values
  error = false;
  string = "Dummy Text";
  string.toUInt64(&error);
  EXPECT_TRUE(error);

  error = false;
  string = "@123";
  string.toUInt64(&error);
  EXPECT_TRUE(error);

  error = false;
  string = "123$";
  string.toUInt64(&error);
  EXPECT_TRUE(error);

  error = false;
  string = "123.90";
  string.toUInt64(&error);
  EXPECT_TRUE(error);

  // valid values
  error = false;
  string = "3890";
  EXPECT_EQ(3890, string.toUInt64(&error));
  EXPECT_FALSE(error);
  
  string = "+4590";
  EXPECT_EQ(4590, string.toUInt64(&error));
  EXPECT_FALSE(error);

  string = "-100";
  EXPECT_EQ(static_cast<u64>(-100), string.toUInt64(&error));
  EXPECT_FALSE(error);

  string = "18446744073709551615";
  EXPECT_EQ(18446744073709551615LL, string.toUInt64(&error));
  EXPECT_FALSE(error);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(StringTest, ToBool)
{
  bool error = false;

  // empty string conversion
  String string;
  string.toBool(&error);
  EXPECT_TRUE(error);

  // invalid values
  error = false;
  string = "Dummy Text";
  string.toBool(&error);
  EXPECT_TRUE(error);

  error = false;
  string = "123";
  string.toBool(&error);
  EXPECT_TRUE(error);

  error = false;
  string = "trueth";
  string.toBool(&error);
  EXPECT_TRUE(error);

  error = false;
  string = "falseth";
  string.toBool(&error);
  EXPECT_TRUE(error);

  // valid values
  error = false;
  string = "true";
  EXPECT_TRUE(string.toBool(&error));
  EXPECT_FALSE(error);

  string = "TRUE";
  EXPECT_TRUE(string.toBool(&error));
  EXPECT_FALSE(error);

  string = "TrUe";
  EXPECT_TRUE(string.toBool(&error));
  EXPECT_FALSE(error);

  string = "false";
  EXPECT_FALSE(string.toBool(&error));
  EXPECT_FALSE(error);

  string = "FALSE";
  EXPECT_FALSE(string.toBool(&error));
  EXPECT_FALSE(error);

  string = "fAlSe";
  EXPECT_FALSE(string.toBool(&error));
  EXPECT_FALSE(error);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(StringTest, ToFloat)
{
  bool error = false;

  // empty string conversion
  String string;
  string.toFloat(&error);
  EXPECT_TRUE(error);

  // invalid values
  error = false;
  string = "Dummy Text";
  string.toFloat(&error);
  EXPECT_TRUE(error);

  error = false;
  string = "@123";
  string.toFloat(&error);
  EXPECT_TRUE(error);

  error = false;
  string = "123$";
  string.toFloat(&error);
  EXPECT_TRUE(error);

  error = false;
  string = "123.12.23";
  string.toFloat(&error);
  EXPECT_TRUE(error);

  // valid values
  error = false;
  string = "3890";
  EXPECT_EQ(3890.0f, string.toFloat(&error));
  EXPECT_FALSE(error);

  string = "4590.12";
  EXPECT_EQ(4590.12f, string.toFloat(&error));
  EXPECT_FALSE(error);

  string = "+4590.12";
  EXPECT_EQ(4590.12f, string.toFloat(&error));
  EXPECT_FALSE(error);

  string = "-0.32";
  EXPECT_EQ(-0.32f, string.toFloat(&error));
  EXPECT_FALSE(error);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(StringTest, ToFloat64)
{
  bool error = false;

  // empty string conversion
  String string;
  string.toFloat64(&error);
  EXPECT_TRUE(error);

  // invalid values
  error = false;
  string = "Dummy Text";
  string.toFloat64(&error);
  EXPECT_TRUE(error);

  error = false;
  string = "@123";
  string.toFloat64(&error);
  EXPECT_TRUE(error);

  error = false;
  string = "123$";
  string.toFloat64(&error);
  EXPECT_TRUE(error);

  error = false;
  string = "123.12.23";
  string.toFloat64(&error);
  EXPECT_TRUE(error);

  // valid values
  error = false;
  string = "3890";
  EXPECT_EQ(3890.0, string.toFloat64(&error));
  EXPECT_FALSE(error);

  string = "4590.12";
  EXPECT_EQ(4590.12, string.toFloat64(&error));
  EXPECT_FALSE(error);

  string = "+4590.12";
  EXPECT_EQ(4590.12, string.toFloat64(&error));
  EXPECT_FALSE(error);

  string = "-0.32";
  EXPECT_EQ(-0.32, string.toFloat64(&error));
  EXPECT_FALSE(error);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(StringTest, Trim)
{
  const char* KWhiteSpaces = " \t\n\r\v\f";

  // string without white spaces on both sides
  EXPECT_STREQ(KReferenceStdString3.c_str(), String(KReferenceStdString3).trimmed().toAscii());

  // string with whitespaces at the begining
  EXPECT_STREQ(KReferenceStdString3.c_str(), (KWhiteSpaces + String(KReferenceStdString3)).trimmed().toAscii());

  // string with whitespaces at the end
  EXPECT_STREQ(KReferenceStdString3.c_str(), (String(KReferenceStdString3) + KWhiteSpaces).trimmed().toAscii());

  // string with whitespaces at both sides
  EXPECT_STREQ(KReferenceStdString3.c_str(), (KWhiteSpaces + String(KReferenceStdString3) + KWhiteSpaces).trimmed().toAscii());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(StringTest, Replace)
{
  String string(KReferenceStdString);

  // replace first characters with the fewer characters
  string.replace(0, 4, "It");
  EXPECT_STREQ("It is STD String", string.toAscii());

  // remove a part of the string
  //string.replace(6, 3, "");
  //EXPECT_STREQ("It is String", string.toAscii());

  // TAGE - it should be possible to test assertion conditions too. Think about it.
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(StringTest, ReplaceAll)
{
  String string(KReferenceStdString3);

  // replace all occurences of word 'is' with the upper cased versions
  string.replaceAll("is", "IS");
  EXPECT_STREQ("That IS yet another STD String which IS cool and it IS really needed as IS.", string.toAscii());

  // replace all 'IS'es with lowercase longer german equivalents
  string.replaceAll(String("IS"), String("ist"));
  EXPECT_STREQ("That ist yet another STD String which ist cool and it ist really needed as ist.", string.toAscii());

  // replace all occurences of non-existing word with something else
  string.replaceAll("Blah", "Whowho");
  EXPECT_STREQ("That ist yet another STD String which ist cool and it ist really needed as ist.", string.toAscii());
  string.replaceAll(String("Blah"), String("Whowho"));
  EXPECT_STREQ("That ist yet another STD String which ist cool and it ist really needed as ist.", string.toAscii());

  // TAGE - it should be possible to test assertion conditions too. Think about it.
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(StringTest, Split)
{
  String string(KReferenceStdString2);

  // split valid string
  StringArray parts = string.split(" ");
  EXPECT_EQ(5, parts.size());
  EXPECT_STREQ("This", parts[0].toAscii());
  EXPECT_STREQ("is", parts[1].toAscii());
  EXPECT_STREQ("another", parts[2].toAscii());
  EXPECT_STREQ("STD", parts[3].toAscii());
  EXPECT_STREQ("String", parts[4].toAscii());

  // split with non-existing separator
  parts = string.split("X");
  EXPECT_EQ(1, parts.size());
  EXPECT_STREQ(string.toAscii(), parts[0].toAscii());

  // split with multicharacter separator
  parts = string.split("is");
  EXPECT_EQ(3, parts.size());
  EXPECT_STREQ("Th", parts[0].toAscii());
  EXPECT_STREQ(" ", parts[1].toAscii());
  EXPECT_STREQ(" another STD String", parts[2].toAscii());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(StringTest, Args)
{
  String string("First %1, Third %3, Second %2");

  // replace first argument
  string = string.arg("string");
  EXPECT_STREQ("First string, Third %3, Second %2", string.toAscii());

  // replace second argument
  string = string.arg(-125);
  EXPECT_STREQ("First string, Third %3, Second -125", string.toAscii());

  // replace third argument
  string = string.arg(1024u);
  EXPECT_STREQ("First string, Third 1024, Second -125", string.toAscii());

  // replace forth argument
  //string = string.arg(123.21f);
  //EXPECT_STREQ("First string, Third 1024, Second -125, Forth 123.21", string.toAscii());

  // try to replace string having no arguments
  string = KReferenceCString;
  string = string.arg("some value");
  EXPECT_STREQ(KReferenceCString, string.toAscii());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(StringTest, FromNumber)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
