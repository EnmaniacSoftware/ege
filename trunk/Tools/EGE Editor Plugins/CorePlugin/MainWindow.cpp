#include "MainWindow.h"
#include "NewProjectWindow.h"
#include "ui_mainwindow.h"
#include "Core.h"
#include "Project.h"
//#include "Resources/ResourceLibrary.h"
//#include "Resources/ResourceItemFactory.h"
//#include "Resources/ResourceLibraryDataModel.h"
//#include "Modules/Fonts/FontManagerWindow.h"
//#include "Config.h"
#include <QMessageBox>
#include <QCloseEvent>
#include <QMenu>
#include <QTextStream>
#include <QFileDialog>
#include <QMdiSubWindow>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
MainWindow* app = NULL;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define MAJOR_VERSION 0
#define MINOR_VERSION 1
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
MainWindow::MainWindow() : QMainWindow(),
                           m_ui(new Ui_MainWindow())/*,
                           m_resourceItemFactory(NULL),
                           m_config(NULL),
                           m_fontManagerWindow(NULL)*/
{
  // setup UI
  m_ui->setupUi(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
MainWindow::~MainWindow()
{
  // save settings
  //saveSettings();

  if (m_ui)
  {
    delete m_ui;
    m_ui = NULL;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Initializes object. */
bool MainWindow::initialize()
{
  Core* core = Core::instance();
  Q_ASSERT(core);

  connect(core, SIGNAL(projectCreated(Project*)), this, SLOT(onProjectCreated(Project*)));

  //if (NULL == (m_resourceLibraryWindow = new ResourceLibraryWindow(this)))
  //{
  //  // error!
  //  return false;
  //}

  //addDockWidget(Qt::LeftDockWidgetArea, m_resourceLibraryWindow);
  //connect(m_resourceLibraryWindow, SIGNAL(visibilityChanged(bool)), this, SLOT(onDockWidgetVisibilityChanged(bool)));

  //if (NULL == (m_config = new Config(this)))
  //{
  //  // error!
  //  return false;
  //}

  //if (NULL == (m_resourceItemFactory = new ResourceItemFactory(this)))
  //{
  //  // error!
  //  return false;
  //}

  //if (NULL == (m_projectFactory = new ProjectFactory(this)))
  //{
  //  // error!
  //  return false;
  //}

  //if (NULL == (m_pluginsManager = new PluginsManager(this)))
  //{
  //  // error!
  //  return false;
  //}

  //QStringList pluginPaths;
  //pluginPaths << "Plugins";
  //m_pluginsManager->setPluginPaths(pluginPaths);
  //m_pluginsManager->loadPlugins();

  //if (NULL == (m_fontManagerWindow = new FontManagerWindow(m_resourceLibraryWindow->model())))
  //{
  //  // error!
  //  return false;
  //}

  // do inital title bar update
  updateTitleBar();

  //// load settings
  //loadSettings();

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when File -> New is selected. */
void MainWindow::on_ActionFileNew_triggered(bool checked)
{
  Q_UNUSED(checked);

  NewProjectWindow dlg(this);

  // show dialog
  dlg.exec();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when File -> Open is selected. */
void MainWindow::on_ActionFileOpen_triggered(bool checked)
{
  Q_UNUSED(checked);
  
  Project* project = NULL;

  // prepare filters
	QString filters = tr("Projects");
	filters += QLatin1String(" (*.ege)");

  // open file selection dialog
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Project"), QString(), filters);
  if (fileName.isEmpty())
  {
    // do nothing
    return;
  }

  QFile file(fileName);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    // error!
    QMessageBox::warning(this, tr("Open Project error"), tr("Could not open project file!"), QMessageBox::Ok);
    return;
  }

  // process input data
  /*QXmlStreamReader stream(&file);
  while (!stream.atEnd())
  {
    QXmlStreamReader::TokenType token = stream.readNext();
    switch (token)
    {
      case QXmlStreamReader::StartElement:

        // check if workspace element
        if ("workspace" == stream.name())
        {
          // check file version
          QString version = stream.attributes().value("version").toString();

          int major = version.section(".", 0, -2).toInt();
          int minor = version.section(".", 1).toInt();

          if ((MAJOR_VERSION != major) || (MINOR_VERSION != minor))
          {
            // error!
            QMessageBox::warning(this, tr("Open Project error"), tr("Invalid project version!"), QMessageBox::Ok);
            return;
          }
        }
        // check if project element
        else if ("project" == stream.name())
        {
          // try to create project
          project = m_projectFactory->createProject(stream.attributes().value("type").toString(), stream.attributes().value("name").toString(),
                                                    stream.attributes().value("path").toString(), this);

          if (NULL == project)
          {
            // error!
            QMessageBox::warning(this, tr("Open Project error"), tr("Could not create project!"), QMessageBox::Ok);
            return;
          }

          // deserialize project
          if (!project->unserialize(stream))
          {
            // error!
            delete project;
            return;
          }
        }
        else if ("resources" == stream.name())
        {
          // deserialize resource library
          if (!m_resourceLibraryWindow->unserialize(stream))
          {
            // error!
            delete project;
            return;
          }
        }
        break;
    }
  }

  if (stream.hasError())
  {
    // error!
    QMessageBox::warning(this, tr("Open Project error"), tr("Could not open project file!"), QMessageBox::Ok);
    close();
    return;
  }

  // store project
  m_project = project;

  // connect
  connect(m_project, SIGNAL(dirtyFlagChanged()), this, SLOT(updateTitleBar()));

  // reset dirty flag
  m_project->setDirty(false);

  // update menus
  updateMenus();

  // emit
  emit projectOpened(m_project);*/
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when File -> Close is selected. */
void MainWindow::on_ActionFileClose_triggered(bool checked)
{
  Q_UNUSED(checked);
  /*
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

    // emit
    emit projectClosed();
  }*/
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
/*  Q_ASSERT(m_project);

  QString output;
  QXmlStreamWriter stream(&output);
  stream.setAutoFormatting(true);
  stream.writeStartDocument();
  stream.writeStartElement("workspace");
  stream.writeAttribute("version", QString("%1.%2").arg(MAJOR_VERSION).arg(MINOR_VERSION));
  bool result = !stream.hasError();
  if (result)
  {
    // save resources
    result = m_resourceLibraryWindow->serialize(stream);
    if (result)
    {
      // save project
      result = m_project->serialize(stream);
    }

    if (result)
    {
      stream.writeEndElement();
      stream.writeEndDocument();
    }
  }

  if (result)
  {
    // save it to file
    QFile file(m_project->fullPath());
    result = file.open(QIODevice::WriteOnly | QIODevice::Text);
    if (result)
    {
      QTextStream out(&file);
      out << output;
    
      // reset dirty flag
      m_project->setDirty(false);
    }
  }

  // check for errors
  if (!result)
  {
    // error!
  }

  // update title bar
  updateTitleBar();*/
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when new project has been created. */
void MainWindow::onProjectCreated(Project* project)
{
  // connect
  connect(project, SIGNAL(dirtyFlagChanged()), this, SLOT(updateTitleBar()));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates title bar. */
void MainWindow::updateTitleBar()
{
  Core* core = Core::instance();
  Q_ASSERT(core);

  QString title;
  if (core->currentProject())
  {
    title = core->currentProject()->name() + " [" + core->currentProject()->typeName() + "] - ";
  }
  title += "Enmaniac Game Engine Editor";

  if (core->currentProject() && core->currentProject()->isDirty())
  {
    title += " *";
  }

  setWindowTitle(title);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
///*! Updates menus. */
//void MainWindow::updateMenus()
//{
//  // File menu
//  QMenu* menu = menuBar()->findChild<QMenu*>("fileMenu");
//  Q_ASSERT(menu);
//
//  QList<QAction*> actions = menu->actions();
//  foreach (QAction* action, actions)
//  {
//    if (("ActionFileClose" == action->objectName()) ||
//        ("ActionFileSave" == action->objectName()))
//    {
//      action->setEnabled(NULL != m_project);
//    }
//  }
//
//  // Project menu
//  menu = menuBar()->findChild<QMenu*>("projectMenu");
//  Q_ASSERT(menu);
//  menu->setEnabled(NULL != m_project);
//
//  actions = menu->actions();
//  foreach (QAction* action, actions)
//  {
//    //if (("ActionFileClose" == action->objectName()) ||
//    //    ("ActionFileSave" == action->objectName()))
//    //{
//    //  action->setEnabled(NULL != m_project);
//    //}
//  }
//
//  // View menu
//  menu = menuBar()->findChild<QMenu*>("viewMenu");
//  Q_ASSERT(menu);
//
//  actions = menu->actions();
//  foreach (QAction* action, actions)
//  {
//    if (("ActionViewResourceLibrary" == action->objectName()))
//    {
//      action->setChecked(m_resourceLibraryWindow->isVisible());
//    }
//  }
//}
////--------------------------------------------------------------------------------------------------------------------------------------------------------------
///*! Slot called when View -> ResourceLibrary is selected. */
//void MainWindow::on_ActionViewResourceLibrary_triggered(bool checked)
//{
//  m_resourceLibraryWindow->setVisible(checked);
//}
////--------------------------------------------------------------------------------------------------------------------------------------------------------------
///*! Saves settings. */
//void MainWindow::saveSettings()
//{
//  m_config->beginGroup("main-window");
//  m_config->setValue("geometry", saveGeometry());
//  m_config->setValue("state", saveState());
//  m_config->endGroup();
//
//  // save resource library setting
//  m_resourceLibraryWindow->saveSettings(m_config);
//
//  // sync
//  m_config->sync();
//}
////--------------------------------------------------------------------------------------------------------------------------------------------------------------
///*! Loads settings. */
//void MainWindow::loadSettings()
//{
//  m_config->beginGroup("main-window");
//  restoreGeometry(m_config->value("geometry").toByteArray());
//  restoreState(m_config->value("state").toByteArray());
//  m_config->endGroup();
//
//  // load resource library setting
//  m_resourceLibraryWindow->loadSettings(m_config);
//
//  // sync
//  m_config->sync();
//}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Event called on application close request. */
void MainWindow::closeEvent(QCloseEvent *event)
{
  Project* project = Core::instance()->currentProject();

  // check if anything to save
  if (project && project->isDirty())
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
///*! Slot called when dock widget changes visibility. */
//void MainWindow::onDockWidgetVisibilityChanged(bool visible)
//{
//  Q_UNUSED(visible);
//
//  updateMenus();
//}
////--------------------------------------------------------------------------------------------------------------------------------------------------------------
///*! Slot called when Project -> Font Manager is selected. */
//void MainWindow::on_ActionProjectFontManager_triggered(bool checked)
//{
//  Q_UNUSED(checked);
//
//  // check if already in MDI area
//  QMdiSubWindow* subWindow = findMdiChild(m_fontManagerWindow->objectName());
//  if (NULL == subWindow)
//  {
//    // add to MDI area
//    subWindow = m_ui->mdiArea->addSubWindow(m_fontManagerWindow);
//  }
//
//  // show it
//  subWindow->show();
//}
////--------------------------------------------------------------------------------------------------------------------------------------------------------------
///*! Returns MDI subwindow with the given name. */
//QMdiSubWindow* MainWindow::findMdiChild(const QString& name) const
//{
//  foreach (QMdiSubWindow* window, m_ui->mdiArea->subWindowList())
//  {
//    // check if found
//    if (name == window->widget()->objectName())
//    {
//      // found
//      return window;
//    }
//  }
//
//  return NULL;
//}
////--------------------------------------------------------------------------------------------------------------------------------------------------------------
