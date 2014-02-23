#include "Application.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
  Application application(argc, argv);

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
