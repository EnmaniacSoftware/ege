#include "Core/Application/Application.h"
#include <EGECommandLine.h>
#include <EGEDictionary.h>

#ifdef EGE_PLATFORM_WIN32
#define COMMANDLINE(name) CommandLineParser name
#else
#define COMMANDLINE(name) CommandLineParser name(argc, argv)
#endif // #ifdef EGE_PLATFORM_WIN32

EGE_NAMESPACE_BEGIN

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

  Dictionary commandLineParams;

  // create application instance
  Application* application = Application::CreateInstance(commandLineParams);
  if (NULL != application)
  {
    // run application
    result = application->run();
  }

  // deinitialize memory manager
  MemoryManager::Deinit();

  return (EGE_SUCCESS == result) ? 0 : 1;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END