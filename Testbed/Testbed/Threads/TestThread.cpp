#include "App.h"
#include "TestThread.h"
#include <EGEDebug.h>

EGE_NAMESPACE

Mutex TestThread::m_mutex = Mutex(NULL);

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TestThread::TestThread(Application* app, void* userData) : Thread(app), 
                                                           m_userData(userData)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TestThread::~TestThread()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Thread override. Work method. */
s32 TestThread::run()
{
  s32* value = reinterpret_cast<s32*>(m_userData);
  
  while (!isStopping())
  {
    // slow loop
    for (int a = 0; a < 200000; a++)
    {
      if (a > 200)
      {
        int b = *value;

        *value = b;
      }
    }

    EGE_ASSERT(m_mutex.lock());
    *value = (*value) + 1;
    EGE_ASSERT(m_mutex.unlock());

    if (*value == 10000)
    {
      break;
    }
  }

  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
