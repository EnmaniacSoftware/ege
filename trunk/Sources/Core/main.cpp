#include "Core/Application/Application.h"
#include <EGECommandLine.h>
#include <EGEDictionary.h>

#ifdef EGE_PLATFORM_WIN32
#define COMMANDLINE(name) CommandLineParser name(reinterpret_cast<char*>(strCmdLine))
#else
#define COMMANDLINE(name) CommandLineParser name(argc, argv)
#endif // #ifdef EGE_PLATFORM_WIN32

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef EGE_PLATFORM_WIN32
#include <windows.h>
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR strCmdLine, INT)
#else
int main(int argc, char** argv)
#endif
{
  // process command line
  COMMANDLINE(commandLineParser);

  EGEResult result = EGE_ERROR;

  // create application instance
  Application* application = Application::CreateInstance();
  if (NULL != application)
  {
    // construct application
    result = application->construct(commandLineParser.dictionary());
    if (EGE_SUCCESS == result)
    {
      // run application
      result = application->run();
    }
  }

  // deinitialize memory manager
  MemoryManager::Deinit();

  return (EGE_SUCCESS == result) ? 0 : 1;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
