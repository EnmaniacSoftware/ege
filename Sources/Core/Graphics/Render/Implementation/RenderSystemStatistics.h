#ifndef EGE_CORE_GRAPHICS_RENDER_RENDERSYSTEMSTATISTICS_H
#define EGE_CORE_GRAPHICS_RENDER_RENDERSYSTEMSTATISTICS_H

/*! Render system statistics component. 
 *  This object is used for gathering debug and performance information about rendering subsystem.
 */

#include "EGE.h"
#include "EGETime.h"
#include "EGEComponent.h"
#include "EGEDynamicArray.h"
#include "EGESignal.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Render queue data struct. */
struct RenderSystemRenderQueueData
{
  s32 priority;
  s32 primitiveType;
  u32 hash;
  u32 batchCount;           /*!< Number of seperate render queues bound to this hash value. */
  u32 vertexCount;          /*!< Number of vertices rendered by all rendered queues bound to this hash value. */
  u32 indexedBatchCount;    /*!< Number of indexed batches only. */
};

/*! Statistics data structure for a single frame. */
struct RenderSystemFrameStatisticData
{
 // u32 m_VBOBufferDataCalls;     /*!< Number of glBufferData calls in current frame. */
 // u32 m_VBOBufferSubDataCalls;  /*!< Number of glBufferSubData calls in current frame. */
  
  u32 drawElementsCalls;                              /*!< Number of glDrawElements calls in current frame. */
  u32 drawArraysCalls;                                /*!< Number of glDrawArrays calls in current frame. */

  s64 renderDuration;                                 /*!< Frame render time (microseconds). */

  u32 batchCount;                                     /*!< Number of batches rendered. */
  u32 vertexCount;                                    /*!< Number of vertices rendered. */

  DynamicArray<RenderSystemRenderQueueData> queues;   /*!< Render queues data. */
};

/*! Statistics data structure for time-continuous quantities. */
struct RenderSystemContinuousStatisticData
{
  u32 fps;                    /*!< Frames per second. */
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(RenderSystemStatistics, PRenderSystemStatistics)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class RenderSystemStatistics : public Component
{
  public:

    RenderSystemStatistics(Application* app, const String& logFileName = "ege-rs-stats.log", u32 recordsCount = 10);
   ~RenderSystemStatistics();

    /*! Returns current data record. */
    RenderSystemFrameStatisticData& currentRecord();
    /*! Returns last data record. */
    const RenderSystemFrameStatisticData& lastRecord() const;
    /*! Returns time-continuous statistics data structure. */
    const RenderSystemContinuousStatisticData& continuousData() const; 
    /*! Dumps records to file. */
    void dumpDataToFile();

  private slots:

    /*! Slot called when rendering of next frame starts. */
    void onRenderStart();
    /*! Slot called when rendering of current frame ends. */
    void onRenderEnd();

  private:

    /*! Clears current record data. */
    void clearCurrentRecord();

  private:

    /*! Array containing data records for a number of render frames. */
    DynamicArray<RenderSystemFrameStatisticData> m_records;
    /*! Current record index. */
    u32 m_currentIndex;
    /*! Log file name. */
    String m_logFileName;
    /*! Time-continuous statistic data structure. */
    RenderSystemContinuousStatisticData m_continuousData;
    /*! Number of frames rendered so far within current FPS interval. 
     *  @note This helper variable to calculate FPS over 1-second intervals.
     */
    s32 m_fpsRendersCount;
    /*! Current FPS interval start time-stamp. 
     *  @note This helper variable to calculate FPS over 1-second intervals.
     */
    s64 m_fpsStartTimeStamp;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_GRAPHICS_RENDER_RENDERSYSTEMSTATISTICS_H