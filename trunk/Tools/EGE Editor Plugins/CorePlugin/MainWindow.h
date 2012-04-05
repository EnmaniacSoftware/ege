#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Ui_MainWindow;
class Project;
class ResourceLibraryWindow;
class Config;
class QCloseEvent;
class ResourceItemFactory;
class ProjectFactory;
class FontManagerWindow;
class QMdiSubWindow;
class PluginsManager;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Application main window. */
class MainWindow : public QMainWindow
{
  Q_OBJECT

  public:

    MainWindow();
   ~MainWindow();

  public:

    /* Initializes object. */
    bool initialize();

  //  /*! Return pointer to currently opened project. */
  //  inline Project* project() const { return m_project; }
  //  /*! Returns pointer to resource item factory. */
  //  inline ResourceItemFactory* resourceItemFactory() const { return m_resourceItemFactory; }
  //  /*! Returns pointer to project factory. */
  //  inline ProjectFactory* projectFactory() const { return m_projectFactory; }
  //  /*! Returns pointer to plugins manager. */
  //  inline PluginsManager* pluginsManager() const { return m_pluginsManager; }

  //private slots:

  //  /* Slot called when File -> New is selected. */
  //  void on_ActionFileNew_triggered(bool checked);
  //  /* Slot called when File -> Open is selected. */
  //  void on_ActionFileOpen_triggered(bool checked);
  //  /* Slot called when File -> Save is selected. */
  //  void on_ActionFileSave_triggered(bool checked);
  //  /* Slot called when File -> Close is selected. */
  //  void on_ActionFileClose_triggered(bool checked);
  //  /* Slot called when File -> Exit is selected. */
  //  void on_ActionFileExit_triggered(bool checked);
  //  /* Slot called when View -> ResourceLibrary is selected. */
  //  void on_ActionViewResourceLibrary_triggered(bool checked);
  //  /* Slot called when Project -> Font Manager is selected. */
  //  void on_ActionProjectFontManager_triggered(bool checked);
  //  /* Slot called when new project has been created. */
  //  void onNewProjectCreated(Project* project);
  //  /* Updates title bar. */
  //  void updateTitleBar();
  //  /* Updates menus. */
  //  void updateMenus();
  //  /* Slot called when dock widget changes visibility. */
  //  void onDockWidgetVisibilityChanged(bool visible);

  //private:

  //  /* Saves settings. */
  //  void saveSettings();
  //  /* Loads settings. */
  //  void loadSettings();
  //  /* Event called on application close request. */
  //  void closeEvent(QCloseEvent *event);
  //  /* Returns MDI subwindow with the given name. */
  //  QMdiSubWindow* findMdiChild(const QString& name) const;

  private:

    /*! Main window UI. */
    Ui_MainWindow* m_ui;
  //  /*! Current project. */
  //  Project* m_project;
  //  /*! Resource library window. */
  //  ResourceLibraryWindow* m_resourceLibraryWindow;
  //  /*! Resource item factory. */
  //  ResourceItemFactory* m_resourceItemFactory;
  //  /*! Config. */
  //  Config* m_config;
  //  /*! Project factory. */
  //  ProjectFactory* m_projectFactory;
  //  /*! Font manager. */
  //  FontManagerWindow* m_fontManagerWindow;
  //  /*! Plugins manager. */
  //  PluginsManager* m_pluginsManager;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // MAIN_WINDOW_H