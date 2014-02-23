#ifndef EGE_CORE_DEBUG_ENGINEINFO_H
#define EGE_CORE_DEBUG_ENGINEINFO_H

/** This object represents info struct for gathering debug and performance information.
 *  @note It is intended to be used internally only.
 */

#include "EGE.h"
#include "EGETime.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class EngineInfo
{
  public:

    EngineInfo();
   ~EngineInfo();

  public:

    /*! Returns instance to object. */
    static EngineInfo& Instance();

  public slots:

    /*! Slot called when reset of data is requested. */
    void onReset();

  public:

    /*! Data structure for callecting API calls statistics. */
    struct DataCalls
    {
      u32 m_VBOBufferDataCalls;     /*!< Number of glBufferData calls in current frame. */
      u32 m_VBOBufferSubDataCalls;  /*!< Number of glBufferSubData calls in current frame. */
      u32 m_drawElementsCalls;      /*!< Number of glDrawElements calls in current frame. */
      u32 m_drawArraysCalls;        /*!< Number of glDrawArrays calls in current frame. */
    };

  public:

    /*! Current frame calls. */
    DataCalls m_current;
    /*! Last frame calls. */
    DataCalls m_last;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#if EGE_FEATURE_ENGINE_INFO
  #define ENGINE_INFO(command) EngineInfo::Instance().m_current.m_##command
#else
  #define ENGINE_INFO(command) ege_noop()
#endif // EGE_FEATURE_ENGINE_INFO

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_DEBUG_ENGINEINFO_H