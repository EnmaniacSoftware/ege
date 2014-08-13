#include "Core/Graphics/Render/Implementation/RenderSystemStatistics.h"
#include "EGEEngine.h"
#include "EGEGraphics.h"
#include "EGEFile.h"
#include "EGEStringBuffer.h"
#include "EGETimer.h"
#include "EGEGraphics.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function returning literal for given render primitive type. */
const char* PrimitiveTypeName(s32 primitiveType)
{
  switch (primitiveType)
  {
    case EGEGraphics::RPT_TRIANGLES:        return "RPT_TRIANGLES";
    case EGEGraphics::RPT_TRIANGLE_STRIPS:  return "RPT_TRIANGLE_STRIPS";
    case EGEGraphics::RPT_TRIANGLE_FAN:     return "RPT_TRIANGLE_FAN";
    case EGEGraphics::RPT_LINES:            return "RPT_LINES";
    case EGEGraphics::RPT_LINE_LOOP:        return "RPT_LINE_LOOP";
    case EGEGraphics::RPT_POINTS:           return "RPT_POINTS";
  
    default:

      EGE_ASSERT_X(false, "Unhandled primitive type!");
      break;
  }

  return "UNKNOWN";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderSystemStatistics::RenderSystemStatistics(Engine& engine, const String& logFileName, u32 recordsCount) 
: Component(EGE_OBJECT_UID_RENDER_SYSTEM_STATISTICS, "rs-statistics")
, m_engine(engine)
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
  ege_connect(engine.graphics(), renderStart, this, RenderSystemStatistics::onRenderStart);
  ege_connect(engine.graphics(), renderEnd, this, RenderSystemStatistics::onRenderEnd);

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
  clearCurrentRecord();

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
void RenderSystemStatistics::dumpDataToFile(bool dumpComponentNames)
{
  StringBuffer buffer;

  // go thru all records backwards
  for (s32 i = 0; i < static_cast<s32>(m_records.size()); ++i)
  {
    const s32 index = (0 <= (m_currentIndex - i - 1)) ? (m_currentIndex - i - 1) : (static_cast<s32>(m_records.size()) + m_currentIndex - i - 1);

    const RenderSystemFrameStatisticData& data = m_records[index];

    buffer << "-- START RECORD\n";
    buffer << "Render time      : " << data.renderDuration << " msec\n";
    buffer << "DrawElementsCalls: " << data.drawElementsCalls << "\n";
    buffer << "DrawArraysCalls  : " << data.drawArraysCalls << "\n";
    buffer << "Batch Count      : " << data.batchCount << "\n";
    buffer << "Vertex Count     : " << data.vertexCount << "\n";

    buffer << "Render queues: " << static_cast<s32>(data.queues.size()) << "\n";
    for (DynamicArray<RenderSystemRenderQueueData>::const_iterator it = data.queues.begin(); it != data.queues.end(); ++it)
    {
      const RenderSystemRenderQueueData& queueData = *it;

      buffer << " Hash: "<< queueData.hash << " Priority: " << queueData.priority << " Primitive: " << PrimitiveTypeName(queueData.primitiveType) 
             << " Batch Count: " << queueData.batchCount << " Indexed Batch Count: " << queueData.indexedBatchCount << " Vertex Count: " 
             << queueData.vertexCount << "\n";
      
      for (StringList::const_iterator itName = queueData.componentNames.begin(); dumpComponentNames && (itName != queueData.componentNames.end()); ++itName)
      {
        buffer << "   " << *itName << "\n";
      }
    }

    buffer << "-- END RECORD\n\n";
  }

  // write to file
  File file(m_logFileName);
  if (EGE_SUCCESS == file.open(EGEFile::MODE_APPEND))
  {
    DataBuffer dataBuffer(buffer.string().toAscii(), buffer.string().length());
    file.write(dataBuffer);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystemStatistics::clearCurrentRecord()
{
  RenderSystemFrameStatisticData& record = currentRecord();

  record.drawElementsCalls = 0;
  record.drawArraysCalls   = 0;
  record.renderDuration    = 0;
  record.batchCount        = 0;
  record.vertexCount       = 0;

  const s32 KRenderQueuesReservedItemCount = 200;
  EGE_ASSERT_X(KRenderQueuesReservedItemCount >= record.queues.size(), "Increase reserve value!");

  record.queues.clear();
  record.queues.reserve(KRenderQueuesReservedItemCount);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Engine& RenderSystemStatistics::engine() const
{
  return m_engine;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END