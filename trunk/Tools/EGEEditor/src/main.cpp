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

    MainWindow mainWindow;

    mainWindow.show();
    return a.exec();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------