#ifndef EGE_CORE_HARDWAREBUFFER_H
#define EGE_CORE_HARDWAREBUFFER_H

/** 
 *   HardwareBuffer base class.
 */

#include <EGE.h>
#include "Core/Data/HardwareBufferTypes.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DECLARE_SMART_CLASS(HardwareBuffer, PHardwareBuffer)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class HardwareBuffer
{
  public:

    HardwareBuffer(Application* app, EGEHardwareBuffer::UsageType usage);
    virtual ~HardwareBuffer();

  protected:

    /*! Usage type. */
    EGEHardwareBuffer::UsageType m_usage;
    /*! Vertex count. */
    u32 m_vertexCount;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_HARDWAREBUFFER_H
