#import "iOS/Application/AppDelegate.h"

//EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
int main(int argc, char** argv)
{
  @autoreleasepool
  {
    return UIApplicationMain(argc, argv, nil, NSStringFromClass([AppDelegate class]));
  }
  
//  // process command line
//  CommandLineParser commandLineParser(argc, argv);
//
//  EGEResult result = EGE_ERROR;
//
//  // create application instance
//  Application* application = Application::CreateInstance();
//  if (NULL != application)
//  {
//    // construct application
//    result = application->construct(commandLineParser.dictionary());
//    if (EGE_SUCCESS == result)
//    {
//      // run application
//      result = application->run();
//    }
//  }
//
//  // deinitialize memory manager
//  MemoryManager::Deinit();
//
//  return (EGE_SUCCESS == result) ? 0 : 1;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
