#include "Core/Application/Application.h"
#include <EGECommandLine.h>
#include <EGEDictionary.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
int main(int argc, char** argv)
{
  EGEResult result = EGE_ERROR;

  // initialize memory manager
  if (MemoryManager::Initialize())
  {
    // process command line
    CommandLineParser commandLineParser(argc, argv);

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
  }

  // deinitialize memory manager
  MemoryManager::Deinitialize();

  return (EGE_SUCCESS == result) ? 0 : 1;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
