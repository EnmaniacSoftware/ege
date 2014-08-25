#ifndef EGE_IOS_ENGINE_ENGINEOBJECT_H
#define EGE_IOS_ENGINE_ENGINEOBJECT_H

/*! Engine implementation for iOS.
 */

#include "Core/Engine/Interface/EngineInstance.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class EngineApplication;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class EngineInstanceIOS : public EngineInstance
{
  public:

    explicit EngineInstanceIOS();
   ~EngineInstanceIOS();

  private:

    /*! @see EngineInstance::contruct. */
    EGEResult construct(const Dictionary& commandLineDictionary) override;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_IOS_ENGINE_ENGINEOBJECT_H