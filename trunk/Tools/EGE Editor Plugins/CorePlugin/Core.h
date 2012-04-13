#ifndef CORE_H
#define CORE_H

#include <QObject>
#include "coreplugin_global.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class MainWindow;
class ProjectFactory;
class Project;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class COREPLUGIN_API Core : public QObject
{
  Q_OBJECT

  public:
  
    Core(QObject* parent = NULL);
   ~Core();

  signals:

    /*! Signal emitted when new project is created. */
    void projectCreated(Project* project);

  public:

    /* Returns core instance. */       
    static Core* instance();
    /* Initializes object. */
    bool initialize();
    /* Deinitializes object. */
    void deinitialize();

  public:

    /* Returns main window. */
    QObject* mainWindow2() const;
    /* Returns main window. */
    MainWindow* mainWindow() const;
    /* Returns project factory. */
    ProjectFactory* projectFactory() const;
    /* Returns current project. */
    Project* currentProject() const;

  public slots:

    /* Slot called when new project is created. */
    void onProjectCreated(Project* project);

  private:

    /*! Main window. */
    MainWindow* m_mainWindow;
    /*! Project factory. */
    ProjectFactory* m_projectFactory;
    /*! Current project. */
    Project* m_project;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // CORE_H
