#include "Core/Graphics/Render/Implementation/RenderSystemStatistics.h"
#include "EGEApplication.h"
#include "EGEGraphics.h"
#include "EGEFile.h"
#include "EGEStringBuffer.h"
#include "EGETimer.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderSystemStatistics::RenderSystemStatistics(Application* app, const String& logFileName, u32 recordsCount) 
: Component(app, EGE_OBJECT_UID_RENDER_SYSTEM_STATISTICS, "rs-statistics")
, m_currentIndex(0)
, m_logFileName(logFileName)
, m_fpsRendersCount(0)
, m_fpsStartTimeStamp(0LL)
{
  // create empty records in front
  while (0 < recordsCount--)
  {
    m_records.push_back(RenderSystemFrameStatisticData());
  }

  // connect
  ege_connect(app->graphics(), renderStart, this, RenderSystemStatistics::onRenderStart);
  ege_connect(app->graphics(), renderEnd, this, RenderSystemStatistics::onRenderEnd);

  // clean up
  EGE_MEMSET(&m_continuousData, 0, sizeof (m_continuousData));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderSystemStatistics::~RenderSystemStatistics()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderSystemFrameStatisticData& RenderSystemStatistics::currentRecord()
{
  return m_records[m_currentIndex];
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const RenderSystemFrameStatisticData& RenderSystemStatistics::lastRecord() const
{
  const s32 index = (0 == m_currentIndex) ? static_cast<s32>(m_records.size() - 1) : static_cast<s32>(m_currentIndex);
  return m_records[index];
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const RenderSystemContinuousStatisticData& RenderSystemStatistics::continuousData() const
{
  return m_continuousData;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystemStatistics::onRenderStart()
{
  // clean up data
  EGE_MEMSET(&currentRecord(), 0, sizeof (RenderSystemFrameStatisticData));

  // store render start time
  currentRecord().renderDuration = Timer::GetMicroseconds();

  // check if 1 second hasnt passed yet
  if (1000000LL > (currentRecord().renderDuration - m_fpsStartTimeStamp))
  {
    // new render
    ++m_fpsRendersCount;
  }
  else
  {
    // store FPS indication
    m_continuousData.fps = m_fpsRendersCount;

    // reset renders count
    m_fpsRendersCount = 0;

    // reset time stamp
    m_fpsStartTimeStamp = currentRecord().renderDuration;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystemStatistics::onRenderEnd()
{
  // calculate render time
  currentRecord().renderDuration = Timer::GetMicroseconds() - currentRecord().renderDuration;

  // move to next index
  ++m_currentIndex %= m_records.size();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystemStatistics::dumpDataToFile()
{
  StringBuffer buffer;

  // go thru all records backwards
  for (s32 i = static_cast<s32>(m_currentIndex); i != static_cast<s32>(m_currentIndex); --i)
  {
    const s32 index = (0 > i) ? static_cast<s32>(m_records.size() - 1) : i;

    const RenderSystemFrameStatisticData& data = m_records[index];

    buffer << "-- START RECORD ---\n";
    buffer << "Render time      : " << data.renderDuration << "microsec\n";
    buffer << "DrawElementsCalls: " << data.drawElementsCalls << "\n";
    buffer << "DrawArraysCalls  : " << data.drawArraysCalls << "\n";
    buffer << "-- END RECORD ---\n";
  }

  File file(m_logFileName);
  if (EGE_SUCCESS == file.open(EGEFile::MODE_APPEND))
  {
    DataBuffer buffer(buffer.string().toAscii(), buffer.string().length());
    file.write(buffer);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
