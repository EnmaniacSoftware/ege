#include "Application.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
  Application application(argc, argv);

  // register resources
///*  if (!QResource::registerResource("res/resources.rcc"))
//  {
//    // error!
//    return -1;
//  }
//

  int result = 1;

  // initialize application
  if (application.initialize())
  {
    // run it
    result = application.exec();
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
