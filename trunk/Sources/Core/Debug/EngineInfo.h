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

    /*! Returns instance to object. */
    static EngineInfo& Instance();

    /*! Updates object. */
    void update(const Time& time);

  public:

    /*! Number of glBufferData calls in current frame. */
    u32 m_VBOBufferDataCalls;
    /*! Number of glBufferSubData calls in current frame. */
    u32 m_VBOBufferSubDataCalls;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#if EGE_FEATURE_ENGINE_INFO
#define ENGINE_INFO(command) EngineInfo::Instance().##command
#else
#define ENGINE_INFO(command) ege_noop()
#endif // EGE_FEATURE_ENGINE_INFO

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_DEBUG_ENGINEINFO_H