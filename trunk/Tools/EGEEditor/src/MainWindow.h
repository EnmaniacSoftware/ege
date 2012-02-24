#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Ui_MainWindow;
class Project;
class ResourceLibraryWindow;
class Config;
class QCloseEvent;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class MainWindow : public QMainWindow
{
  Q_OBJECT

  public:

    MainWindow();
   ~MainWindow();

  signals:

    /*! Signal emitted when new project has been created. */
    void projectCreated();
    /*! Signal emitted when existing project has been opened. */
    void projectOpened();
    /*! Signal emitted when project was closed. */
    void projectClosed();

  public:

    /*! Return pointer to currently opened project. */
    inline Project* project() const { return m_project; }

  private slots:

    /* Slot called when File -> New is selected. */
    void on_ActionFileNew_triggered(bool checked);
    /* Slot called when File -> Open is selected. */
    void on_ActionFileOpen_triggered(bool checked);
    /* Slot called when File -> Save is selected. */
    void on_ActionFileSave_triggered(bool checked);
    /* Slot called when File -> Close is selected. */
    void on_ActionFileClose_triggered(bool checked);
    /* Slot called when File -> Exit is selected. */
    void on_ActionFileExit_triggered(bool checked);
    /* Slot called when View -> ResourceLibrary is selected. */
    void on_ActionViewResourceLibrary_triggered(bool checked);
    /* Slot called when new project has been created. */
    void onNewProjectCreated(Project* project);
    /* Updates title bar. */
    void updateTitleBar();
    /* Updates menus. */
    void updateMenus();
    /* Slot called when dock widget changes visibility. */
    void onDockWidgetVisibilityChanged(bool visible);

  private:

    /* Saves settings. */
    void saveSettings();
    /* Loads settings. */
    void loadSettings();

    void closeEvent(QCloseEvent *event);

  private:

    /*! Main window UI. */
    Ui_MainWindow* m_ui;
    /*! Current project. */
    Project* m_project;
    /*! Resource library. */
    ResourceLibraryWindow* m_resourceLibraryWindow;
    /*! Config. */
    Config* m_config;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // MAIN_WINDOW_H