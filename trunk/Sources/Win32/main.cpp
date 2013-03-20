#include "Core/Application/Application.h"
#include <EGECommandLine.h>
#include <EGEDictionary.h>
#include <windows.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR strCmdLine, INT)
{
  // process command line
  CommandLineParser commandLineParser(strCmdLine);

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

  // clean up
  Application::DestroyInstance(application);

  // deinitialize memory manager
  MemoryManager::Deinit();

  return (EGE_SUCCESS == result) ? 0 : 1;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
