#ifndef EGE_CORE_AUDIO_AUDIOMANAGERFACTORY_H
#define EGE_CORE_AUDIO_AUDIOMANAGERFACTORY_H

/*! Factory for audio manager instances.
 */

#include "EGE.h"
#include "EGEEngine.h"
#include "Core/Engine/Interface/EngineModule.h"
#include "Core/Factory/Interface/Factory.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class AudioManagerFactory : public Factory<EngineModule<IAudioManager>*>
{
  public:

    AudioManagerFactory(Engine& engine);
    virtual ~AudioManagerFactory();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_AUDIO_AUDIOMANAGERFACTORY_H