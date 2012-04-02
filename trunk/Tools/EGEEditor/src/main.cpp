#include <QApplication>
#include <QResource>
#include "MainWindow.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  // register resources
  if (!QResource::registerResource("res/resources.rcc"))
  {
    // error!
    return -1;
  }

  // allocate main window
  app = new MainWindow();
  if (NULL == app)
  {
    // error!
    return -2;
  }

  // initialize
  if (!app->initialize())
  {
    // error!
    return -3;
  }

  // show main window
  app->show();

  // run
  int result = a.exec();

  // clean up
  delete app;
  app = NULL;

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------