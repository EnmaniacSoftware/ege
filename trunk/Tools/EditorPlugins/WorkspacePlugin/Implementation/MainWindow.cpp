#include "MainWindow.h"
#include "NewProjectWindow.h"
#include <Projects/Project.h>
#include <ObjectPool.h>
//#include "ProjectFactory.h"
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
#include <QLabel>
#include <QDebug>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define MAJOR_VERSION 0
#define MINOR_VERSION 1
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
MainWindow::MainWindow() : QMainWindow()
                           /*m_resourceItemFactory(NULL),
                           m_config(NULL),
                           m_fontManagerWindow(NULL)*/
{
  // setup UI
  setupUi(this);

  // connect
  connect(ObjectPool::Instance(), SIGNAL(objectAdded(QObject*)), this, SLOT(onObjectAdded(QObject*)));
  connect(ObjectPool::Instance(), SIGNAL(objectRemoved(QObject*)), this, SLOT(onObjectRemoved(QObject*)));

  // do inital title bar update
  updateTitleBar();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
MainWindow::~MainWindow()
{
  // save settings
  //saveSettings();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool MainWindow::initialize()
{
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

//  // do inital title bar update
//  updateTitleBar();

  //// load settings
  //loadSettings();

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void MainWindow::on_ActionFileNew_triggered(bool checked)
{
  Q_UNUSED(checked);

  NewProjectWindow dlg(this);

  // connect
  connect(&dlg, SIGNAL(projectCreated(Project*)), this, SLOT(updateTitleBar()));

  // show dialog
  dlg.exec();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void MainWindow::on_ActionFileOpen_triggered(bool checked)
{
  Q_UNUSED(checked);
  
//  ObjectPool* pool = ObjectPool::Instance();

//  ProjectFactory* projectFactory = pool->findChild<ProjectFactory*>();
//  if (NULL == projectFactory)
//  {
//    // done
//    qWarning() << Q_FUNC_INFO << "No ProjectFactory found";
//    return;
//  }

//  Project* project = NULL;

//  // prepare filters
//	QString filters = tr("Projects");
//	filters += QLatin1String(" (*.ege)");

//  // open file selection dialog
//	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Project"), QString(), filters);
//  if (fileName.isEmpty())
//  {
//    // do nothing
//    return;
//  }

//  QFile file(fileName);
//  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
//  {
//    // error!
//    QMessageBox::warning(this, tr("Open Project error"), tr("Could not open project file!"), QMessageBox::Ok);
//    return;
//  }

//  // process input data
//  QXmlStreamReader stream(&file);
//  while (!stream.atEnd())
//  {
//    QXmlStreamReader::TokenType token = stream.readNext();
//    switch (token)
//    {
//      case QXmlStreamReader::StartElement:

//        // check if workspace element
//        if ("workspace" == stream.name())
//        {
//          // check file version
//          QString version = stream.attributes().value("version").toString();

//          int major = version.section(".", 0, -2).toInt();
//          int minor = version.section(".", 1).toInt();

//          if ((MAJOR_VERSION != major) || (MINOR_VERSION != minor))
//          {
//            // error!
//            QMessageBox::warning(this, tr("Open Project error"), tr("Invalid project version!"), QMessageBox::Ok);
//            return;
//          }
//        }
//        // check if project element
//        else if ("project" == stream.name())
//        {
//          // try to create project
//          project = projectFactory->createProject(stream.attributes().value("type").toString(), stream.attributes().value("name").toString(),
//                                                  stream.attributes().value("path").toString(), this);

//          if (NULL == project)
//          {
//            // error!
//            QMessageBox::warning(this, tr("Open Project error"), tr("Could not create project!"), QMessageBox::Ok);
//            return;
//          }

//          // deserialize project
//          if (!project->unserialize(stream))
//          {
//            // error!
//            delete project;
//            return;
//          }
//        }
//        break;
//    }
//  }

//  if (stream.hasError())
//  {
//    // error!
//    QMessageBox::warning(this, tr("Open Project error"), tr("Could not open project file!"), QMessageBox::Ok);
//    close();
//    return;
//  }

  //// store project
  //m_project = project;

  //// connect
  //connect(m_project, SIGNAL(dirtyFlagChanged()), this, SLOT(updateTitleBar()));

  //// reset dirty flag
  //m_project->setDirty(false);

  // update menus
  //updateMenus();

  //// emit
  //emit projectOpened(m_project);*/
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void MainWindow::on_ActionFileClose_triggered(bool checked)
{
  Q_UNUSED(checked);

  Project* project = ObjectPool::Instance()->getObject<Project>();
  Q_ASSERT(project);

  // save settings
  //saveSettings();

  // close project
  closeProject();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void MainWindow::on_ActionFileExit_triggered(bool checked)
{
  Q_UNUSED(checked);

  // close project
  if (closeProject())
  {
    // quit
    qApp->quit();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void MainWindow::on_ActionFileSave_triggered(bool checked)
{
  Q_UNUSED(checked);

//  Project* project = ObjectPool::Instance()->getObject<Project>();
//  Q_ASSERT(project);

//  QString output;
//  QXmlStreamWriter stream(&output);
//  stream.setAutoFormatting(true);
//  stream.writeStartDocument();
//  stream.writeStartElement("workspace");
//  stream.writeAttribute("version", QString("%1.%2").arg(MAJOR_VERSION).arg(MINOR_VERSION));
//  bool result = !stream.hasError();
//  if (result)
//  {
//    // save project
//    result = project->serialize(stream);

//    stream.writeEndElement();
//    stream.writeEndDocument();
//  }

//  if (result)
//  {
//    // save it to file
//    QFile file(project->fullPath());
//    result = file.open(QIODevice::WriteOnly | QIODevice::Text);
//    if (result)
//    {
//      QTextStream out(&file);
//      out << output;
//    }
//  }

//  // check for errors
//  if (!result)
//  {
//    // error!
//    QMessageBox::critical(this, tr("Save Error!"), tr("Error occured during the save process!"));
//  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void MainWindow::onObjectAdded(QObject* object)
{
  // check if project added
  if (qobject_cast<Project*>(object))
  {
    // connect for notification
    connect(object, SIGNAL(dirtyFlagChanged()), this, SLOT(updateTitleBar()));

    // update title bar
    updateTitleBar();

    // update menus
    updateMenus();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void MainWindow::onObjectRemoved(QObject* object)
{
  // check if project removed
  if (qobject_cast<Project*>(object))
  {
    // update title
    updateTitleBar();

    // update menus
    updateMenus();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void MainWindow::updateTitleBar()
{
  QString title;

  Project* project = ObjectPool::Instance()->getObject<Project>();
  if (NULL != project)
  {
    title = project->name() + " [" + project->typeName() + "] - ";
  }
  title += tr("Enmaniac Game Engine Editor");

  if ((NULL != project) && project->isDirty())
  {
    title += " *";
  }

  setWindowTitle(title);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void MainWindow::addChildWindow(QWidget* widget)
{
  mdiArea->addSubWindow(widget);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void MainWindow::removeChildWindow(QWidget* widget)
{
  mdiArea->removeSubWindow(widget);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void MainWindow::updateMenus()
{
  Project* project = ObjectPool::Instance()->getObject<Project>();

  // File menu
  QMenu* menu = this->menu(EFile);

  QList<QAction*> actions = menu->actions();
  foreach (QAction* action, actions)
  {
    if (("ActionFileClose" == action->objectName()) ||
        ("ActionFileSave" == action->objectName()))
    {
      action->setEnabled(NULL != project);
    }
  }

  // Project menu
  menu =this->menu(EProject);
  menu->setEnabled(NULL != project);

  actions = menu->actions();
  foreach (QAction* action, actions)
  {
    //if (("ActionFileClose" == action->objectName()) ||
    //    ("ActionFileSave" == action->objectName()))
    //{
    //  action->setEnabled(NULL != m_project);
    //}
  }

  // View menu
//  menu = menuBar()->findChild<QMenu*>("viewMenu");
//  Q_ASSERT(menu);

//  actions = menu->actions();
//  foreach (QAction* action, actions)
//  {
//    if (("ActionViewResourceLibrary" == action->objectName()))
//    {
//      action->setChecked(m_resourceLibraryWindow->isVisible());
//    }
//  }
}
////--------------------------------------------------------------------------------------------------------------------------------------------------------------
//void MainWindow::on_ActionViewResourceLibrary_triggered(bool checked)
//{
//  m_resourceLibraryWindow->setVisible(checked);
//}
////--------------------------------------------------------------------------------------------------------------------------------------------------------------
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
void MainWindow::closeEvent(QCloseEvent *event)
{
  if ( ! closeProject())
  {
    event->ignore();
  }
  else
  {
    event->accept();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//void MainWindow::onDockWidgetVisibilityChanged(bool visible)
//{
//  Q_UNUSED(visible);
//
//  updateMenus();
//}
////--------------------------------------------------------------------------------------------------------------------------------------------------------------
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
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool MainWindow::closeProject()
{
  Project* project = ObjectPool::Instance()->getObject<Project>();

  // close project if any
  if (NULL != project)
  {
    // show warning
    if (project->isDirty())
    {
      // prompt
      if (QMessageBox::No == QMessageBox::warning(this, tr("Project not saved"),
                                                  tr("Project contains changes which have not been saved yet!\n\nDo you want to close anyway ?"),
                                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
      {
        // dont close
        return false;
      }
    }

    // remove from pool
    ObjectPool::Instance()->removeObject(project);

    // delete
    delete project;
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QMenu* MainWindow::menu(Menu menu) const
{
  QString menuName;
  switch (menu)
  {
    case EFile:    menuName = "fileMenu"; break;
    case EView:    menuName = "viewMenu"; break;
    case EActions: menuName = "actionsMenu"; break;
    case EProject: menuName = "projectMenu"; break;

    default:
      break;
  }

  QMenu* menuObject = menuBar()->findChild<QMenu*>(menuName);
  Q_ASSERT(menuObject);

  return menuObject;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
