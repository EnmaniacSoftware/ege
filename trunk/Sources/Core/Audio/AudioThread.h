#ifndef EGE_CORE_AUDIO_THREAD_H
#define EGE_CORE_AUDIO_THREAD_H

/*! Audio thread.. 
 */

#include <EGEThread.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class AudioThread : public Thread
{
  public:

    AudioThread(Application* app);
   ~AudioThread();

  private:

    /* Thread override. Work method. */
    EGE::s32 run() override;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_AUDIO_THREAD_H