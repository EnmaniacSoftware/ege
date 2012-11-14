#ifndef EGE_CORE_AUDIO_THREAD_OPENAL_H
#define EGE_CORE_AUDIO_THREAD_OPENAL_H

/*! Audio thread for OpenAL implementation
 */

#include <EGEThread.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class AudioManagerPrivate;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class AudioThreadOpenAL : public Thread
{
  public:

    AudioThreadOpenAL(Application* app, AudioManagerPrivate* manager);
   ~AudioThreadOpenAL();

  private:

    /* Thread override. Work method. */
    EGE::s32 run() override;

  private:

    /* Audio manager instance. */
    AudioManagerPrivate* m_manager;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_AUDIO_THREAD_OPENAL_H