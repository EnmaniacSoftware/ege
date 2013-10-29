#ifndef WORKSPACE_MAINWINDOW_H
#define WORKSPACE_MAINWINDOW_H

/*! Application's main window. */

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Project;
class Config;
class QCloseEvent;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class MainWindow : public QMainWindow, public Ui::MainWindow
{
  Q_OBJECT

  public:

    MainWindow();
   ~MainWindow();

  public:

    /*! Available menu types. */
    enum Menu
    {
      EFile = 0,
      EView,
      EActions,
      EProject
    };

  public:

    /*! Initializes object. */
    bool initialize();
    /*! Adds child window. */
    void addChildWindow(QWidget* widget);
    /*! Removes child window. */
    void removeChildWindow(QWidget* widget);
    /*! Returns pointer given menu.
     *  @param  menu  Kind of menu to retrive.
     *  @return Pointer to given menu. NULL if error occured.
     */
    QMenu* menu(Menu menu) const;

  //  /*! Returns pointer to resource item factory. */
  //  inline ResourceItemFactory* resourceItemFactory() const { return m_resourceItemFactory; }

  private slots:

    /*! Slot called when File -> New is selected. */
    void on_ActionFileNew_triggered(bool checked);
    /*! Slot called when File -> Open is selected. */
    void on_ActionFileOpen_triggered(bool checked);
    /*! Slot called when File -> Save is selected. */
    void on_ActionFileSave_triggered(bool checked);
    /*! Slot called when File -> Close is selected. */
    void on_ActionFileClose_triggered(bool checked);
    /*! Slot called when File -> Exit is selected. */
    void on_ActionFileExit_triggered(bool checked);
  //  /* Slot called when View -> ResourceLibrary is selected. */
  //  void on_ActionViewResourceLibrary_triggered(bool checked);
  //  /* Slot called when Project -> Font Manager is selected. */
  //  void on_ActionProjectFontManager_triggered(bool checked);

    /*! Slot called when new object has been added into the pool. */
    void onObjectAdded(QObject* object);
    /*! Slot called when object is about to be removed from pool. */
    void onObjectRemoved(QObject* object);

    /*! Updates title bar. */
    void updateTitleBar();
    /*! Updates menus. */
    void updateMenus();
  //  /* Slot called when dock widget changes visibility. */
  //  void onDockWidgetVisibilityChanged(bool visible);

  private:

  //  /* Saves settings. */
  //  void saveSettings();
  //  /* Loads settings. */
  //  void loadSettings();
    /*! Event called on application close request. */
    void closeEvent(QCloseEvent *event);
    /*! Closes the project.
     *  @return TRUE if project has been sucessfully closed. Otherwise FALSE.
     *  @note If project is to be saved, prompt is shown. Project is deleted and removed from ObjectPool.
     */
    bool closeProject();
  //  /* Returns MDI subwindow with the given name. */
  //  QMdiSubWindow* findMdiChild(const QString& name) const;

  private:

  //  /*! Resource item factory. */
  //  ResourceItemFactory* m_resourceItemFactory;
  //  /*! Config. */
  //  Config* m_config;
  //  /*! Font manager. */
  //  FontManagerWindow* m_fontManagerWindow;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // WORKSPACE_MAINWINDOW_H
