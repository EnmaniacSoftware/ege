#ifndef EGE_CORE_HARDWAREBUFFERTYPES_H
#define EGE_CORE_HARDWAREBUFFERTYPES_H

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "Core/Platform.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

namespace EGEHardwareBuffer
{
  /*! Buffer available usages. */
  enum UsageType
  {
    UT_STATIC_WRITE   = 1,                  /*!< Created once, used many times. Data will be sent from application to GL. */
    UT_DYNAMIC_WRITE  = 2,                  /*!< Frequently changable. Data will be sent from application to GL. */
    UT_DISCARDABLE    = 4                   /*!< Flag indicating that content before it is overwritten is completely out-of-interest to us. 
                                                 This allows for some optimization. Makes sense with UT_DYNAMIC_WRITE. */                  
  };
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_HARDWAREBUFFERTYPES_H