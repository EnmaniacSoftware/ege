#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Ui_MainWindow;
class Project;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class MainWindow : public QMainWindow
{
  Q_OBJECT

  public:

    MainWindow();
    ~MainWindow();

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
    /* Slot called when new project has been created. */
    void onNewProjectCreated(Project* project);

  private:

    /*! Main window UI. */
    Ui_MainWindow* m_ui;
    /*! Current project. */
    Project* m_project;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // MAIN_WINDOW_H