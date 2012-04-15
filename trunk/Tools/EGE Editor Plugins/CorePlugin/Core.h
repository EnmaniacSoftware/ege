#ifndef CORE_H
#define CORE_H

#include <QObject>
#include "coreplugin_global.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class MainWindow;
class ProjectFactory;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class COREPLUGIN_API Core : public QObject
{
  Q_OBJECT

  public:
  
    Core(QObject* parent = NULL);
   ~Core();

  public:

    /* Returns core instance. */       
    static Core* Instance();

  public:

    /* Initializes object. */
    bool initialize();
    /* Deinitializes object. */
    void deinitialize();

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
