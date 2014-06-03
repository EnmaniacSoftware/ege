#ifndef EGE_CORE_AUDIO_OPENAL_AUDIOTHREADOPENAL_H
#define EGE_CORE_AUDIO_OPENAL_AUDIOTHREADOPENAL_H

/*! Audio thread for OpenAL implementation
 */

#include "EGEThread.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class AudioManagerOpenAL;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class AudioThreadOpenAL : public Thread
{
  public:

    AudioThreadOpenAL(Application* app, AudioManagerOpenAL* manager);
   ~AudioThreadOpenAL();

  private:

    /* Thread override. Work method. */
    EGE::s32 run() override;

  private:

    /* Audio manager instance. */
    AudioManagerOpenAL* m_manager;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_AUDIO_OPENAL_AUDIOTHREADOPENAL_H