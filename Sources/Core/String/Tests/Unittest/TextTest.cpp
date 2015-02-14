#include "TestFramework/Interface/TestBase.h"
#include <EGEMemory.h>
#include <EGEText.h>
#include <string>
#include <algorithm>
#include <cctype>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const char* KReferenceCString               = "This is C text";
static const Char* KReferenceWideCString           = L"This is wide C text";
static const std::wstring KReferenceWideStdString  = L"This is wide STD text";
static const std::wstring KReferenceWideStdString2 = L"This is another wide STD text";
static const std::wstring KReferenceWideStdString3 = L"That is yet another wide STD text which is cool and it is really needed as is.";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define EGE_EXPECT_WSTREQ(expected, actual) EXPECT_EQ(0, memcmp(expected, actual, wcslen(expected) * sizeof (Char)))
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class TextTest : public TestBase
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
void TextTest::SetUpTestCase()
{
  EXPECT_TRUE(MemoryManager::Initialize());
  }
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void TextTest::TearDownTestCase()
{
  MemoryManager::Deinitialize();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void TextTest::SetUp()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void TextTest::TearDown()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(TextTest, Construct)
{
  Text empty;
  EXPECT_EQ(0, empty.length());

  Text empty2(reinterpret_cast<char*>(NULL));
  EXPECT_EQ(0, empty2.length());

  Text empty3(reinterpret_cast<char*>(NULL), 10);
  EXPECT_EQ(0, empty3.length());

  Text empty4(reinterpret_cast<Char*>(NULL));
  EXPECT_EQ(0, empty4.length());

  Text empty5(reinterpret_cast<Char*>(NULL), 10);
  EXPECT_EQ(0, empty5.length());

  Text fromConstChar(KReferenceCString);
  for (s32 i = 0; i < static_cast<s32>(strlen(KReferenceCString)); ++i)
  {
    EXPECT_EQ(KReferenceCString[i], fromConstChar.data()[i]);
  }

  Text fromConstCharShort(KReferenceCString, 5);
  for (s32 i = 0; i < 5; ++i)
  {
    EXPECT_EQ(KReferenceCString[i], fromConstChar.data()[i]);
  }

  Text fromWideCString(KReferenceWideCString);
  EGE_EXPECT_WSTREQ(KReferenceWideCString, fromWideCString.data());

  Text fromWideCStringShort(KReferenceWideCString, 5);
  EXPECT_EQ(0, memcmp(KReferenceWideCString, fromWideCStringShort.data(), 5));

  Text fromAnotherString(fromConstChar);
  EGE_EXPECT_WSTREQ(fromAnotherString.data(), fromConstChar.data());

  Text fromWideSTDString(KReferenceWideStdString);
  EGE_EXPECT_WSTREQ(KReferenceWideStdString.data(), fromWideSTDString.data());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(TextTest, Assignment)
{
  Text string;

  // operators
  string = Text(KReferenceWideStdString);
  EGE_EXPECT_WSTREQ(KReferenceWideStdString.data(), string.data());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(TextTest, Equality)
{
  Text string1(KReferenceCString);
  Text string2(string1);

  // operators
  EXPECT_TRUE(string1 == string2);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(TextTest, Inequality)
{
  Text string1(KReferenceCString);
  Text string2(string1);

  // operators
  EXPECT_FALSE(string1 != string2);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(TextTest, Append)
{
  Text string1(KReferenceWideStdString);
  Text string2(KReferenceWideStdString2);

  // operators +=
  Text strings;
  strings += string1;
  EGE_EXPECT_WSTREQ(string1.data(), strings.data());

  strings += string2;
  EGE_EXPECT_WSTREQ((KReferenceWideStdString + KReferenceWideStdString2).data(), strings.data());

  // operators +
  strings = string1 + string2;
  EGE_EXPECT_WSTREQ((KReferenceWideStdString + KReferenceWideStdString2).data(), strings.data());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(TextTest, Length)
{
  Text string;
  EXPECT_EQ(0, string.length());

  Text string2("");
  EXPECT_EQ(0, string2.length());

  Text string3(KReferenceWideStdString);
  EXPECT_EQ(static_cast<s32>(KReferenceWideStdString.size()), string3.length());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(TextTest, Clear)
{
  Text string(KReferenceWideStdString);
  EXPECT_EQ(static_cast<s32>(KReferenceWideStdString.size()), string.length());

  string.clear(); 
  EXPECT_EQ(0, string.length());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(TextTest, IsEmpty)
{
  Text string;
  EXPECT_TRUE(string.isEmpty());

  Text string2("");
  EXPECT_TRUE(string2.isEmpty());

  Text string3(KReferenceWideStdString);
  EXPECT_FALSE(string3.isEmpty());

  string3 = "";
  EXPECT_TRUE(string3.isEmpty());

  Text string4(KReferenceWideStdString);
  EXPECT_FALSE(string4.isEmpty());

  string4.clear();
  EXPECT_TRUE(string4.isEmpty());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(TextTest, IndexOf)
{
  const Char* KSubString            = L"ST";
  const Char* KNonExistingSubString = L"blah";
  const s32 KSubStringPosition = static_cast<s32>(KReferenceWideStdString.find(KSubString, 0));
  EXPECT_TRUE(0 <= KSubStringPosition);

  // find existing substring starting from the begining
  Text string(KReferenceWideStdString);
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
TEST_F(TextTest, At)
{
  Text string(KReferenceWideStdString);
  for (s32 i = 0; i < static_cast<s32>(KReferenceWideStdString.size()); ++i)
  {
    EXPECT_EQ(string.at(i), KReferenceWideStdString[i]);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(TextTest, Format)
{
  Text string;

  // %c
  string.format("This is an %c.", 'a');
  EGE_EXPECT_WSTREQ(L"This is an a.", string.data());

  // %s
  string.format("This is an %s.", "Alabama");
  EGE_EXPECT_WSTREQ(L"This is an Alabama.", string.data());

  // %d
  string.format("This is %d.", 125);
  EGE_EXPECT_WSTREQ(L"This is 125.", string.data());
  string.format("This is %d.", -234);
  EGE_EXPECT_WSTREQ(L"This is -234.", string.data());
  string.format("This is %d.", -2147483647 - 1);
  EGE_EXPECT_WSTREQ(L"This is -2147483648.", string.data());
  string.format("This is %d.", 2147483647);
  EGE_EXPECT_WSTREQ(L"This is 2147483647.", string.data());

  // %f
  // TAGE - add
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(TextTest, ToLower)
{
  Text string(KReferenceWideStdString2);

  // create reference lower-case string
  std::wstring reference = KReferenceWideStdString2;
  std::transform(reference.begin(), reference.end(), reference.begin(), (int(*)(int)) std::tolower);

  EGE_EXPECT_WSTREQ(reference.c_str(), string.toLower().data());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(TextTest, ToUpper)
{
  Text string(KReferenceWideStdString2);

  // create reference upper-case string
  std::wstring reference = KReferenceWideStdString2;
  std::transform(reference.begin(), reference.end(), reference.begin(), (int(*)(int)) std::toupper);

  EGE_EXPECT_WSTREQ(reference.c_str(), string.toUpper().data());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(TextTest, Data)
{
  Text string(KReferenceWideStdString2);

  EGE_EXPECT_WSTREQ(KReferenceWideStdString2.c_str(), string.data());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(TextTest, Replace)
{
  Text string(KReferenceWideStdString);

  // replace first characters with the fewer characters
  string.replace(0, 4, "It");
  EGE_EXPECT_WSTREQ(L"It is wide STD text", string.data());

  // remove a part of the string
  //string.replace(6, 3, "");
  //EXPECT_STREQ("It is String", string.toAscii());

  // TAGE - it should be possible to test assertion conditions too. Think about it.
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(TextTest, ReplaceAll)
{
  Text string(KReferenceWideStdString3);

  // replace all occurences of word 'is' with the upper cased versions
  string.replaceAll("is", "IS");
  EGE_EXPECT_WSTREQ(L"That IS yet another wide STD text which IS cool and it IS really needed as IS.", string.data());

  // replace all 'IS'es with lowercase longer german equivalents
  string.replaceAll(Text("IS"), Text("ist"));
  EGE_EXPECT_WSTREQ(L"That ist yet another wide STD text which ist cool and it ist really needed as ist.", string.data());

  // replace all occurences of non-existing word with something else
  string.replaceAll("Blah", "Whowho");
  EGE_EXPECT_WSTREQ(L"That ist yet another wide STD text which ist cool and it ist really needed as ist.", string.data());
  string.replaceAll(Text("Blah"), Text("Whowho"));
  EGE_EXPECT_WSTREQ(L"That ist yet another wide STD text which ist cool and it ist really needed as ist.", string.data());

  // TAGE - it should be possible to test assertion conditions too. Think about it.
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(TextTest, Args)
{
  Text string("First %1, Third %3, Second %2");

  // replace first argument
  string = string.arg("string");
  EGE_EXPECT_WSTREQ(L"First string, Third %3, Second %2", string.data());

  // replace second argument
  string = string.arg(-125);
  EGE_EXPECT_WSTREQ(L"First string, Third %3, Second -125", string.data());

  // replace third argument
  //string = string.arg(1024u);
  //EGE_EXPECT_WSTREQ(L"First string, Third 1024, Second -125", string.data());

  // replace forth argument
  //string = string.arg(123.21f);
  //EGE_EXPECT_WSTREQ(L"First string, Third 1024, Second -125, Forth 123.21", string.data());

  // try to replace string having no arguments
  string = KReferenceWideCString;
  string = string.arg("some value");
  EGE_EXPECT_WSTREQ(KReferenceWideCString, string.data());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(TextTest, FromNumber)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
