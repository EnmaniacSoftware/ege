#include "TestFramework/Interface/TestBase.h"
#include <EGEMemory.h>
#include <EGEDebug.h>
#include <EGEString.h>
#include <string>
#include <limits>
#include <vector>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const char* KInvalidTag = "InvalidTag";
static const char* KValidTag   = "ValidTag";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class BufferDebug : public Debug
{
  public:

    BufferDebug(DebugMessageType type, const String& name) : Debug(type, name) {}
    BufferDebug(const Debug& other) : Debug(other) {}

  public:

    /*! Returns warning message type prefix. */
    static const char* WarningPrefix()
    { 
      return KWarningPrefix; 
    }
    /*! Returns error message type prefix. */
    static const char* ErrorPrefix() 
    { 
      return KErrorPrefix; 
    }

  public:

    /*! Returns data stored in the buffer. */
    String data() const
    {
      return (NULL != buffer()) ? buffer()->string() : "";
    }
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class DebugTest : public TestBase
{
  protected:

    static void SetUpTestCase();
    static void TearDownTestCase();

  protected:

    virtual void SetUp();
    virtual void TearDown();

  protected:

    /*! Verifies various logging primitives.
     *  @param  messageType Debug message type to use while logging.
     *  @param  space       TRUE if logging is to contain spaces after each logged primitive.
     */
    void verifyLoggingPrimitives(DebugMessageType messageType, bool space) const;

  private:

    /*! Prepares string.
     *  @param  text        Input string to be prepared.
     *  @param  messageType Debug message type to use while logging.
     *  @param  space       TRUE if space should be appended to input string.
     *  @return Prepared string.
     */
    std::string prepareString(std::string text, DebugMessageType messageType, bool space) const;
    /*! Prepares debug logging object. 
     *  @param  messageType Debug message type to use while logging.
     *  @param  space       TRUE if space should be appended to each logged primitive.
     */
    BufferDebug prepareDebugLogger(DebugMessageType messageType, bool spacing) const;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void DebugTest::SetUpTestCase()
{
  EXPECT_TRUE(MemoryManager::Initialize());
  
  // enable some name for debugging
  StringList list(KValidTag);
  Debug::EnableNames(list);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void DebugTest::TearDownTestCase()
{
  MemoryManager::Deinitialize();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void DebugTest::SetUp()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void DebugTest::TearDown()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void DebugTest::verifyLoggingPrimitives(DebugMessageType messageType, bool space) const
{
  // bool (TRUE)
  BufferDebug debug = prepareDebugLogger(messageType, space);
  debug << true;
  EXPECT_STREQ(prepareString("true", messageType, space).c_str(), debug.data().toAscii());

  // bool (FALSE)
  debug = prepareDebugLogger(messageType, space);
  debug << false;
  EXPECT_STREQ(prepareString("false", messageType, space).c_str(), debug.data().toAscii());

  // s16
  std::vector<s16> valuesS16;
  valuesS16.push_back(std::numeric_limits<s16>::min());
  valuesS16.push_back(-4765);
  valuesS16.push_back(-823);
  valuesS16.push_back(-42);
  valuesS16.push_back(-7);
  valuesS16.push_back(0);
  valuesS16.push_back(9);
  valuesS16.push_back(73);
  valuesS16.push_back(852);
  valuesS16.push_back(9374);
  valuesS16.push_back(17460);
  valuesS16.push_back(std::numeric_limits<s16>::max());

  for (std::vector<s16>::const_iterator it = valuesS16.begin(); it != valuesS16.end(); ++it)
  {
    s16 value = *it;

    debug = prepareDebugLogger(messageType, space);
    debug << value;

    char buffer[16];
    sprintf_s(buffer, sizeof (buffer), "%d", value);

    EXPECT_STREQ(prepareString(buffer, messageType, space).c_str(), debug.data().toAscii());
  }

  // u16
  std::vector<u16> valuesU16;
  valuesU16.push_back(std::numeric_limits<u16>::min());
  valuesU16.push_back(39);
  valuesU16.push_back(712);
  valuesU16.push_back(9836);
  valuesU16.push_back(12345);
  valuesU16.push_back(45038);
  valuesU16.push_back(std::numeric_limits<u16>::max());

  for (std::vector<u16>::const_iterator it = valuesU16.begin(); it != valuesU16.end(); ++it)
  {
    u16 value = *it;

    debug = prepareDebugLogger(messageType, space);
    debug << value;

    char buffer[16];
    sprintf_s(buffer, sizeof (buffer), "%u", value);

    EXPECT_STREQ(prepareString(buffer, messageType, space).c_str(), debug.data().toAscii());
  }

  // s32
  std::vector<s32> valuesS32;
  valuesS32.push_back(std::numeric_limits<s32>::min());
  valuesS32.push_back(-273927249);
  valuesS32.push_back(-19237263);
  valuesS32.push_back(-8473520);
  valuesS32.push_back(-173847);
  valuesS32.push_back(-39374);
  valuesS32.push_back(-8373);
  valuesS32.push_back(-293);
  valuesS32.push_back(-93);
  valuesS32.push_back(-1);
  valuesS32.push_back(0);
  valuesS32.push_back(29);
  valuesS32.push_back(383);
  valuesS32.push_back(9037);
  valuesS32.push_back(10284);
  valuesS32.push_back(193283);
  valuesS32.push_back(3489934);
  valuesS32.push_back(94034742);
  valuesS32.push_back(563934043);
  valuesS32.push_back(std::numeric_limits<s32>::max());

  for (std::vector<s32>::const_iterator it = valuesS32.begin(); it != valuesS32.end(); ++it)
  {
    s32 value = *it;

    debug = prepareDebugLogger(messageType, space);
    debug << value;

    char buffer[16];
    sprintf_s(buffer, sizeof (buffer), "%d", value);

    EXPECT_STREQ(prepareString(buffer, messageType, space).c_str(), debug.data().toAscii());
  }

  // u32
  std::vector<u32> valuesU32;
  valuesU32.push_back(std::numeric_limits<u32>::min());
  valuesU32.push_back(8);
  valuesU32.push_back(12);
  valuesU32.push_back(937);
  valuesU32.push_back(3740);
  valuesU32.push_back(23846);
  valuesU32.push_back(739201);
  valuesU32.push_back(3937393);
  valuesU32.push_back(97352821);
  valuesU32.push_back(390299372);
  valuesU32.push_back(std::numeric_limits<u32>::max());

  for (std::vector<u32>::const_iterator it = valuesU32.begin(); it != valuesU32.end(); ++it)
  {
    u32 value = *it;

    debug = prepareDebugLogger(messageType, space);
    debug << value;

    char buffer[16];
    sprintf_s(buffer, sizeof (buffer), "%u", value);

    EXPECT_STREQ(prepareString(buffer, messageType, space).c_str(), debug.data().toAscii());
  }

  // s64
  std::vector<s64> valuesS64;
  valuesS64.push_back(std::numeric_limits<s64>::min());
  valuesS64.push_back(-827382937262830928LL);
  valuesS64.push_back(-33739230372920193LL);
  valuesS64.push_back(-3023902930283680LL);
  valuesS64.push_back(-943874832384734LL);
  valuesS64.push_back(-29383894473544LL);
  valuesS64.push_back(-3094809374478LL);
  valuesS64.push_back(-129498394764LL);
  valuesS64.push_back(-50953874374LL);
  valuesS64.push_back(-3738278743LL);
  valuesS64.push_back(-129398324LL);
  valuesS64.push_back(-83984378LL);
  valuesS64.push_back(-7434739LL);
  valuesS64.push_back(-223382LL);
  valuesS64.push_back(-30948LL);
  valuesS64.push_back(-4839LL);
  valuesS64.push_back(-934LL);
  valuesS64.push_back(-34LL);
  valuesS64.push_back(-8LL);
  valuesS64.push_back(0LL);
  valuesS64.push_back(2LL);
  valuesS64.push_back(23LL);
  valuesS64.push_back(933LL);
  valuesS64.push_back(2663LL);
  valuesS64.push_back(30932LL);
  valuesS64.push_back(943763LL);
  valuesS64.push_back(2039732LL);
  valuesS64.push_back(29379983LL);
  valuesS64.push_back(485739372LL);
  valuesS64.push_back(1028363283LL);
  valuesS64.push_back(30384738223LL);
  valuesS64.push_back(398470203844LL);
  valuesS64.push_back(4093872987332LL);
  valuesS64.push_back(39038847892443LL);
  valuesS64.push_back(309344979873233LL);
  valuesS64.push_back(3094873477367382LL);
  valuesS64.push_back(19838237749374400LL);
  valuesS64.push_back(583872303483036393LL);
  valuesS64.push_back(std::numeric_limits<s64>::max());

  for (std::vector<s64>::const_iterator it = valuesS64.begin(); it != valuesS64.end(); ++it)
  {
    s64 value = *it;

    debug = prepareDebugLogger(messageType, space);
    debug << value;

    char buffer[32];
    sprintf_s(buffer, sizeof (buffer), "%lld", value);

    EXPECT_STREQ(prepareString(buffer, messageType, space).c_str(), debug.data().toAscii());
  }

  // u64
  std::vector<u64> valuesU64;
  valuesU64.push_back(std::numeric_limits<u64>::min());
  valuesU64.push_back(2LL);
  valuesU64.push_back(43LL);
  valuesU64.push_back(123LL);
  valuesU64.push_back(9999LL);
  valuesU64.push_back(12345LL);
  valuesU64.push_back(458300LL);
  valuesU64.push_back(7253937LL);
  valuesU64.push_back(92863527LL);
  valuesU64.push_back(302-38373LL);
  valuesU64.push_back(9238463836LL);
  valuesU64.push_back(45283946383LL);
  valuesU64.push_back(338373947324LL);
  valuesU64.push_back(9338373263876LL);
  valuesU64.push_back(28387448734534LL);
  valuesU64.push_back(948947394637883LL);
  valuesU64.push_back(3948023447346874LL);
  valuesU64.push_back(29483864673623930LL);
  valuesU64.push_back(836473789387325323LL);
  valuesU64.push_back(4373638920373734736LL);
  valuesU64.push_back(std::numeric_limits<u64>::max());

  for (std::vector<u64>::const_iterator it = valuesU64.begin(); it != valuesU64.end(); ++it)
  {
    u64 value = *it;

    debug = prepareDebugLogger(messageType, space);
    debug << value;

    char buffer[32];
    sprintf_s(buffer, sizeof (buffer), "%llu", value);

    EXPECT_STREQ(prepareString(buffer, messageType, space).c_str(), debug.data().toAscii());
  }

  // float
  std::vector<float32> valuesFloat;
  valuesFloat.push_back(-34245.34f);
  valuesFloat.push_back(-8344.20f);
  valuesFloat.push_back(-234.93f);
  valuesFloat.push_back(-83.234f);
  valuesFloat.push_back(-9.2234f);
  valuesFloat.push_back(0.0f);
  valuesFloat.push_back(6.5f);
  valuesFloat.push_back(83.2f);
  valuesFloat.push_back(342.0f);
  valuesFloat.push_back(3937.99f);
  valuesFloat.push_back(19373.93f);

  for (std::vector<float32>::const_iterator it = valuesFloat.begin(); it != valuesFloat.end(); ++it)
  {
    float32 value = *it;

    debug = prepareDebugLogger(messageType, space);
    debug << value;

    char buffer[32];
    sprintf_s(buffer, sizeof (buffer), "%f", value);

    EXPECT_STREQ(prepareString(buffer, messageType, space).c_str(), debug.data().toAscii());
  }

  // const char*
  const std::string KStringValue = "This is test string!";
  debug = prepareDebugLogger(messageType, space);
  debug << KStringValue;
  EXPECT_STREQ(prepareString(KStringValue, messageType, space).c_str(), debug.data().toAscii());

  // String
  debug = prepareDebugLogger(messageType, space);
  debug << String(KStringValue);
  EXPECT_STREQ(prepareString(KStringValue, messageType, space).c_str(), debug.data().toAscii());

  // const void *
  // NOTE: reuse U32 data
  for (std::vector<u32>::const_iterator it = valuesU32.begin(); it != valuesU32.end(); ++it)
  {
    const void* value = reinterpret_cast<const void*>(*it);

    debug = prepareDebugLogger(messageType, space);
    debug << value;

    char buffer[16];
    sprintf_s(buffer, sizeof (buffer), "%p", value);

    EXPECT_STREQ(prepareString(buffer, messageType, space).c_str(), debug.data().toAscii());
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
std::string DebugTest::prepareString(std::string text, DebugMessageType messageType, bool space) const
{
  std::string out;

  // add message type prefix
  switch (messageType)
  {
    case EWarning: out = BufferDebug::WarningPrefix(); break;
    case EError:   out = BufferDebug::ErrorPrefix(); break;

    default:
      break;
  }

  out += text;

  if (space)
  {
    out += " ";
  }

  return out;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BufferDebug DebugTest::prepareDebugLogger(DebugMessageType messageType, bool spacing) const
{
  BufferDebug debug(messageType, KValidTag);
  
  if ( ! spacing)
  {
    debug.nospace();
  }

  return debug;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(DebugTest, NotEnabledTags)
{
  // test NORMAL level message
  BufferDebug debug = BufferDebug(ENormal, KInvalidTag);
  debug << "This normal message should be blocked";
  EXPECT_STREQ("", debug.data().toAscii());

  // test WARNING level message
  debug = BufferDebug(EWarning, KInvalidTag);
  debug << "This warning message should be blocked";
  EXPECT_STREQ("", debug.data().toAscii());

  // test ERROR level message
  debug = BufferDebug(EWarning, KInvalidTag);
  debug << "This error message should be blocked";
  EXPECT_STREQ("", debug.data().toAscii());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(DebugTest, LoggingWithoutSpaces)
{
  verifyLoggingPrimitives(ENormal, false);
  verifyLoggingPrimitives(EWarning, false);
  verifyLoggingPrimitives(EError, false);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(DebugTest, LoggingWithSpaces)
{
  verifyLoggingPrimitives(ENormal, true);
  verifyLoggingPrimitives(EWarning, true);
  verifyLoggingPrimitives(EError, true);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
