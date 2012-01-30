#include "MainWindow.h"
#include "NewProjectDlg.h"
#include "Project.h"
#include "ui_mainwindow.h"
#include "ResourceLibrary.h"
#include "Config.h"
#include <QMessageBox>
#include <QCloseEvent>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define MAJOR_VERSION 0
#define MINOR_VERSION 1
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
MainWindow::MainWindow() : QMainWindow(),
                           m_ui(new Ui_MainWindow()),
                           m_project(NULL),
                           m_config(new Config())
{
  // setup UI
  m_ui->setupUi(this);

  // allocate
  m_resourceLibrary = new ResourceLibrary(this);
  addDockWidget(Qt::LeftDockWidgetArea, m_resourceLibrary);

  // do inital title bar update
  updateTitleBar();

  // load settings
  loadSettings();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
MainWindow::~MainWindow()
{
  // save settings
  saveSettings();

  if (m_ui)
  {
    delete m_ui;
    m_ui = NULL;
  }

  if (m_config)
  {
    delete m_config;
    m_config = NULL;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when File -> New is selected. */
void MainWindow::on_ActionFileNew_triggered(bool checked)
{
  Q_UNUSED(checked);

  NewProjectDialog dlg(this);

  // connect for notifications
  connect(&dlg, SIGNAL(projectCreated(Project*)), this, SLOT(onNewProjectCreated(Project*)));

  // show dialog
  dlg.exec();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when File -> Open is selected. */
void MainWindow::on_ActionFileOpen_triggered(bool checked)
{
  Q_UNUSED(checked);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when File -> Close is selected. */
void MainWindow::on_ActionFileClose_triggered(bool checked)
{
  Q_UNUSED(checked);

  // save settings
  saveSettings();

  // close project
  if (m_project)
  {
    // show warning
    if (m_project->isDirty())
    {
      // prompt
      if (QMessageBox::No == QMessageBox::warning(this, tr("Project not saved"), 
                                                  tr("Project contains changes which have not been saved yet!\n\nDo you want to close anyway ?"),
                                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
      {
        // dont close
        return;
      }
    }

    delete m_project;
    m_project = NULL;

    // update title bar
    updateTitleBar();

    // update menus
    updateMenus();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when File -> Exit is selected. */
void MainWindow::on_ActionFileExit_triggered(bool checked)
{
  Q_UNUSED(checked);

  close();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when File -> Save is selected. */
void MainWindow::on_ActionFileSave_triggered(bool checked)
{
  Q_UNUSED(checked);

  // check if anything to save
  if (m_project && m_project->isDirty())
  {
    // save
    if (!m_project->save())
    {
      // TAGE - error!
    }

    // update title bar
    updateTitleBar();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when new project has been created. */
void MainWindow::onNewProjectCreated(Project* project)
{
  // store pointer
  m_project = project;

  // connect
  connect(m_project, SIGNAL(dirtyFlagChanged()), this, SLOT(updateTitleBar()));

  // update title bar
  updateTitleBar();

  // update menus
  updateMenus();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates title bar. */
void MainWindow::updateTitleBar()
{
  QString title;
  if (m_project)
  {
    title = m_project->name() + " - ";
  }
  title += "Enmaniac Game Engine Editor";

  if (m_project && m_project->isDirty())
  {
    title += " *";
  }

  setWindowTitle(title);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates menus. */
void MainWindow::updateMenus()
{
  // File menu
  QMenu* menu = menuBar()->findChild<QMenu*>("fileMenu");
  Q_ASSERT(menu);

  QList<QAction*> actions = menu->actions();
  foreach (QAction* action, actions)
  {
    if (("ActionFileClose" == action->objectName()) ||
        ("ActionFileSave" == action->objectName()))
    {
      action->setEnabled(NULL != m_project);
    }
  }

  // Project menu
  menu = menuBar()->findChild<QMenu*>("projectMenu");
  Q_ASSERT(menu);
  menu->setEnabled(NULL != m_project);

  actions = menu->actions();
  foreach (QAction* action, actions)
  {
    //if (("ActionFileClose" == action->objectName()) ||
    //    ("ActionFileSave" == action->objectName()))
    //{
    //  action->setEnabled(NULL != m_project);
    //}
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when View -> ResourceLibrary is selected. */
void MainWindow::on_ActionResourceLibrary_triggered(bool checked)
{
  int a = 1;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Saves settings. */
void MainWindow::saveSettings()
{
  m_config->beginGroup("main-window");
  m_config->setValue("size", size());
  m_config->setValue("pos", pos());
  m_config->endGroup();

  // save resource library setting
  m_resourceLibrary->saveSettings(m_config);

  // sync
  m_config->sync();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Loads settings. */
void MainWindow::loadSettings()
{
  m_config->beginGroup("main-window");
  resize(m_config->value("size", size()).toSize());
  move(m_config->value("pos", pos()).toPoint());
  m_config->endGroup();

  // load resource library setting
  m_resourceLibrary->loadSettings(m_config);

  // sync
  m_config->sync();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
 void MainWindow::closeEvent(QCloseEvent *event)
 {
  // check if anything to save
  if (m_project && m_project->isDirty())
  {
    // show warning
    if (QMessageBox::Yes != QMessageBox::warning(this, tr("Project not saved"), 
                                                 tr("Project contains changes which have not been saved yet!\n\nDo you want to quit anyway ?"),
                                                 QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
    {
      // dont close
      event->ignore();
      return;
    }
  }

  event->accept();
 }
 //--------------------------------------------------------------------------------------------------------------------------------------------------------------