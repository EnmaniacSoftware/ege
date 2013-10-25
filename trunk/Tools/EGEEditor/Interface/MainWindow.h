#ifndef EDITOR_MAIN_WINDOW_H
#define EDITOR_MAIN_WINDOW_H

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

    /*! Initializes object. */
    bool initialize();
    /*! Adds child window. */
    void addChildWindow(QWidget* widget);
    /*! Removes child window. */
    void removeChildWindow(QWidget* widget);

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
  //  /* Updates menus. */
  //  void updateMenus();
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
     *  @note  If project is to be saved, prompt is shown.
     */
    bool close();
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

#endif // EDITOR_MAIN_WINDOW_H
