#ifndef CORE_H
#define CORE_H

#include "coreplugin_global.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class MainWindow;
class ProjectFactory;
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
    /* Returns project factory. */
    ProjectFactory* projectFactory() const;

  private:

    /*! Main window. */
    MainWindow* m_mainWindow;
    /*! Project factory. */
    ProjectFactory* m_projectFactory;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // CORE_H
