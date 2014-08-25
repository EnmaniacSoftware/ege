#ifndef EGE_WIN32_ENGINE_ENGINEOBJECT_H
#define EGE_WIN32_ENGINE_ENGINEOBJECT_H

/*! Engine implementation for Windows OS.
 */

#include "Core/Engine/Interface/EngineInstance.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class EngineApplication;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class EngineInstanceWin32 : public EngineInstance
{
  public:

    explicit EngineInstanceWin32();
   ~EngineInstanceWin32();

  private:

    /*! @see EngineInstance::contruct. */
    EGEResult construct(const Dictionary& commandLineDictionary) override;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_WIN32_ENGINE_ENGINEOBJECT_H