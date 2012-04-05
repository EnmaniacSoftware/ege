#ifndef CORE_H
#define CORE_H

#include "coreplugin_global.h"
#include "MainWindow.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class COREPLUGIN_API Core
{
  public:
  
    Core();
   ~Core();

  public:

    /* Returns core instance. */       
    static Core* instance();

  public:

    /* Returns main window. */
    MainWindow* mainWindow() const;

  private:

    /*! Main window. */
    MainWindow* m_mainWindow;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // CORE_H
