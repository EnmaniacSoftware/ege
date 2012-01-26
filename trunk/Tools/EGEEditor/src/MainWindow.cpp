#include "MainWindow.h"
#include "NewProjectDlg.h"
#include "Project.h"
#include "ui_mainwindow.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
MainWindow::MainWindow() : QMainWindow(),
                           m_ui(new Ui_MainWindow()),
                           m_project(NULL)
{
  m_ui->setupUi(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
MainWindow::~MainWindow()
{
  if (m_ui)
  {
    delete m_ui;
    m_ui = NULL;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when File -> New is selected. */
void MainWindow::on_ActionFileNew_triggered(bool checked)
{
  NewProjectDialog dlg(this);

  // connect for notifications
  connect(&dlg, SIGNAL(projectCreated(Project*)), this, SLOT(onNewProjectCreated(Project*)));

  // show dialog
  if (QDialog::Rejected == dlg.exec())
  {
    // do nothing
    return;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when File -> Open is selected. */
void MainWindow::on_ActionFileOpen_triggered(bool checked)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when File -> Close is selected. */
void MainWindow::on_ActionFileClose_triggered(bool checked)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when File -> Exit is selected. */
void MainWindow::on_ActionFileExit_triggered(bool checked)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when File -> Save is selected. */
void MainWindow::on_ActionFileSave_triggered(bool checked)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when new project has been created. */
void MainWindow::onNewProjectCreated(Project* project)
{
  m_project = project;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
