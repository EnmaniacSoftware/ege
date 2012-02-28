#ifndef TEST_THREAD_H
#define TEST_THREAD_H

#include <EGEThread.h>
#include <EGEMutex.h>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class TestThread : public EGE::Thread
{
  public:

    TestThread(EGE::Application* app, void* userData);
    ~TestThread();

  private:

    /* Thread override. Work method. */
    EGE::s32 run() override;

  private:

    void* m_userData;

    static EGE::Mutex m_mutex;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // TEST_THREAD_H