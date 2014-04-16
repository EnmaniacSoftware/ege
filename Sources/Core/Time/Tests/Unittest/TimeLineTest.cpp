#include "TestFramework/Interface/TestBase.h"
#include <EGETimeLine.h>
#include <vector>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class TimeLineTest : public TestBase
{
  public:

    /*! Slot called when time line has reached an end (an no looping is done). */
    void onFinished();
    /*! Slot called when frame changes. */
    void onFrameChanged(s32 frameId);

  protected:

    static void SetUpTestCase();
    static void TearDownTestCase();

  protected:

    virtual void SetUp();
    virtual void TearDown();

  protected:

    /*! Number of finished calls. */
    u32 m_finishedCount;
    /*! Changed frames. */
    std::vector<s32> m_frames;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void TimeLineTest::SetUpTestCase()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void TimeLineTest::TearDownTestCase()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void TimeLineTest::SetUp()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void TimeLineTest::TearDown()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void TimeLineTest::onFinished()
{
  ++m_finishedCount;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void TimeLineTest::onFrameChanged(s32 frameId)
{
  m_frames.push_back(frameId);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(TimeLineTest, SingleForward)
{
  const float32 KDuration = 1.0f;

  // setup (default loop count which is 1)
  TimeLine timeLine(NULL);
  timeLine.setDuration(KDuration);
  timeLine.setStartFrame(0);
  timeLine.setEndFrame(10);

  // start
  timeLine.start();
  EXPECT_EQ(TimeLine::STATE_RUNNING, timeLine.state());

  // simulate
  const float32 KTimeStep = 0.1f;
  float32 time = 0;
  while (KDuration >= time)
  {
    // verify state
    EXPECT_EQ(TimeLine::STATE_RUNNING, timeLine.state());

    // advance time
    time += KTimeStep;
    timeLine.update(KTimeStep);
  }

  // verify state
  EXPECT_EQ(TimeLine::STATE_IDLE, timeLine.state());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(TimeLineTest, LoopedForward)
{
  const float32 KDuration = 1.0f;
  const s32 KLoopCount = 2;

  // setup
  TimeLine timeLine(NULL);
  timeLine.setDuration(KDuration);
  timeLine.setStartFrame(0);
  timeLine.setEndFrame(10);
  timeLine.setLoopCount(KLoopCount);

  // start
  timeLine.start();
  EXPECT_EQ(TimeLine::STATE_RUNNING, timeLine.state());

  // simulate
  const float32 KTimeStep = 0.1f;
  float32 time = 0;
  while (KDuration >= time)
  {
    // verify state
    EXPECT_EQ(TimeLine::STATE_RUNNING, timeLine.state());

    // advance time
    time += KTimeStep;
    timeLine.update(KTimeStep);
  }

  // verify state
  EXPECT_EQ(TimeLine::STATE_RUNNING, timeLine.state());

  while ((KLoopCount * KDuration) >= time)
  {
    // verify state
    EXPECT_EQ(TimeLine::STATE_RUNNING, timeLine.state());

    // advance time
    time += KTimeStep;
    timeLine.update(KTimeStep);
  }

  // verify state
  EXPECT_EQ(TimeLine::STATE_IDLE, timeLine.state());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(TimeLineTest, Stop)
{
  const float32 KDuration = 1.0f;

  // setup
  TimeLine timeLine(NULL);
  timeLine.setDuration(KDuration);
  timeLine.setStartFrame(0);
  timeLine.setEndFrame(10);

  // start
  timeLine.start();
  EXPECT_EQ(TimeLine::STATE_RUNNING, timeLine.state());

  // simulate up to half of the time only
  const float32 KTimeStep = 0.1f;
  float32 time = 0;
  while (KDuration * 0.5f >= time)
  {
    // verify state
    EXPECT_EQ(TimeLine::STATE_RUNNING, timeLine.state());

    // advance time
    time += KTimeStep;
    timeLine.update(KTimeStep);
  }

  // stop
  timeLine.stop();

  // verify state
  EXPECT_EQ(TimeLine::STATE_IDLE, timeLine.state());

  // advance time
  while (KDuration >= time)
  {
    // verify state
    EXPECT_EQ(TimeLine::STATE_IDLE, timeLine.state());

    // advance time
    time += KTimeStep;
    timeLine.update(KTimeStep);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(TimeLineTest, SingleFinished)
{
  const float32 KDuration = 1.0f;

  // setup
  TimeLine timeLine(NULL);
  timeLine.setDuration(KDuration);
  timeLine.setStartFrame(0);
  timeLine.setEndFrame(10);

  // connect
  ege_connect(&timeLine, finished, this, TimeLineTest::onFinished);

  // clean up counters
  m_finishedCount = 0;

  // start
  timeLine.start();
  EXPECT_EQ(TimeLine::STATE_RUNNING, timeLine.state());

  // simulate
  const float32 KTimeStep = 0.1f;
  float32 time = 0;
  while (KDuration >= time)
  {
    // verify state
    EXPECT_EQ(TimeLine::STATE_RUNNING, timeLine.state());

    // advance time
    time += KTimeStep;
    timeLine.update(KTimeStep);
  }

  // verify state
  EXPECT_EQ(TimeLine::STATE_IDLE, timeLine.state());

  // verify finished event
  EXPECT_EQ(1, m_finishedCount);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(TimeLineTest, LoopedFinished)
{
  const float32 KDuration = 1.0f;
  const s32 KLoopCount = 20;

  // setup
  TimeLine timeLine(NULL);
  timeLine.setDuration(KDuration);
  timeLine.setStartFrame(0);
  timeLine.setEndFrame(10);
  timeLine.setLoopCount(KLoopCount);

  // connect
  ege_connect(&timeLine, finished, this, TimeLineTest::onFinished);

  // clean up counters
  m_finishedCount = 0;

  // start
  timeLine.start();
  EXPECT_EQ(TimeLine::STATE_RUNNING, timeLine.state());

  // simulate
  const float32 KTimeStep = 0.1f;
  float32 time = 0;
  while ((KLoopCount * KDuration) >= time)
  {
    // verify state
    EXPECT_EQ(TimeLine::STATE_RUNNING, timeLine.state());

    // advance time
    time += KTimeStep;
    timeLine.update(KTimeStep);
  }

  // verify state
  EXPECT_EQ(TimeLine::STATE_IDLE, timeLine.state());

  // verify finished event
  EXPECT_EQ(1, m_finishedCount);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(TimeLineTest, SingleLoopFramesChanged)
{
  const float32 KDuration = 1.0f;
  const s32 KFrameCount = 5;

  // setup
  TimeLine timeLine(NULL);
  timeLine.setDuration(KDuration);
  timeLine.setStartFrame(0);
  timeLine.setEndFrame(KFrameCount);

  // connect
  ege_connect(&timeLine, finished, this, TimeLineTest::onFinished);
  ege_connect(&timeLine, frameChanged, this, TimeLineTest::onFrameChanged);

  // clean up counters
  m_finishedCount = 0;
  m_frames.clear();

  // start
  timeLine.start();
  EXPECT_EQ(TimeLine::STATE_RUNNING, timeLine.state());

  // simulate
  const float32 KTimeStep = 0.1f;
  float32 time = 0;
  while (KDuration >= time)
  {
    // verify state
    EXPECT_EQ(TimeLine::STATE_RUNNING, timeLine.state());

    // advance time
    time += KTimeStep;
    timeLine.update(KTimeStep);
  }

  // verify state
  EXPECT_EQ(TimeLine::STATE_IDLE, timeLine.state());

  // verify finished event
  EXPECT_EQ(1, m_finishedCount);

  // verify frames
  EXPECT_EQ(KFrameCount, static_cast<s32>(m_frames.size()));

  for (s32 i = 0; i < KFrameCount; ++i)
  {
    EXPECT_EQ(i + 1, m_frames[i]);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_F(TimeLineTest, MultipleLoopFramesChanged)
{
  const float32 KDuration = 1.0f;
  const s32 KFrameCount = 5;
  const s32 KLoopCount  = 2;

  // setup
  TimeLine timeLine(NULL);
  timeLine.setDuration(KDuration);
  timeLine.setStartFrame(0);
  timeLine.setEndFrame(KFrameCount);
  timeLine.setLoopCount(KLoopCount);

  // connect
  ege_connect(&timeLine, finished, this, TimeLineTest::onFinished);
  ege_connect(&timeLine, frameChanged, this, TimeLineTest::onFrameChanged);

  // clean up counters
  m_finishedCount = 0;
  m_frames.clear();

  // start
  timeLine.start();
  EXPECT_EQ(TimeLine::STATE_RUNNING, timeLine.state());

  // simulate
  const float32 KTimeStep = 0.1f;
  float32 time = 0;
  while ((KLoopCount * KDuration) >= time)
  {
    // verify state
    EXPECT_EQ(TimeLine::STATE_RUNNING, timeLine.state());

    // advance time
    time += KTimeStep;
    timeLine.update(KTimeStep);
  }

  // verify state
  EXPECT_EQ(TimeLine::STATE_IDLE, timeLine.state());

  // verify finished event
  EXPECT_EQ(1, m_finishedCount);

  // verify frames
  const s32 KExpectedNumberOfFrames = KLoopCount * KFrameCount + (KLoopCount - 1);
  EXPECT_EQ(KExpectedNumberOfFrames, static_cast<s32>(m_frames.size()));

  for (s32 i = 0; i < KExpectedNumberOfFrames; ++i)
  {
    EXPECT_EQ((i + 1) % (KFrameCount + 1), m_frames[i]);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------