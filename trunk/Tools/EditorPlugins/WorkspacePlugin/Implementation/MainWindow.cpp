#include "MainWindow.h"
#include "NewProjectWindow.h"
#include "ui_mainwindow.h"
#include <ObjectPool.h>
#include <FileSystemUtils.h>
#include <Settings.h>
#include <Projects/Project.h>
#include <Projects/ProjectFactory.h>
#include <QMessageBox>
#include <QCloseEvent>
#include <QMenu>
#include <QTextStream>
#include <QFileDialog>
#include <QMdiSubWindow>
#include <QLabel>
#include <QDebug>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const int KMajorVersion    = 0;
static const int KMinorVersion    = 1;
static const int KRevisionVersion = 1;
static const QString KWorkspaceTag              = "Workspace";
static const QString KMajorVersionAttribute     = "version-major";
static const QString KMinorVersionAttribute     = "version-minor";
static const QString KRevisionVersionAttribute  = "version-revision";
static const QString KProjectFileExtension      = "egeproj";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
MainWindow::MainWindow() : QMainWindow(),
                           m_ui(new Ui_MainWindow())
{
  // setup UI
  m_ui->setupUi(this);

  // connect
  connect(ObjectPool::Instance(), SIGNAL(objectAdded(QObject*)), this, SLOT(onObjectAdded(QObject*)));
  connect(ObjectPool::Instance(), SIGNAL(objectRemoved(QObject*)), this, SLOT(onObjectRemoved(QObject*)));

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

  // TAGE - check if any project is active and close it
  Q_ASSERT(NULL == ObjectPool::Instance()->getObject<Project>());

  ProjectFactory* projectFactory = ObjectPool::Instance()->getObject<ProjectFactory>();
  if (NULL == projectFactory)
  {
    // done
    qWarning() << Q_FUNC_INFO << "No ProjectFactory found";
    return;
  }

  // prepare filters
  QString filters = tr("Projects");
  filters += QString(" (*." + KProjectFileExtension + ")");

  // open file selection dialog
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open Project"), QString(), filters);
  if (fileName.isEmpty())
  {
    // do nothing
    return;
  }

  QFile file(fileName);
  if ( ! file.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    // error!
    QMessageBox::warning(this, tr("Open Project error"), tr("Could not open project file!"), QMessageBox::Ok);
    return;
  }

  // process input data
  QXmlStreamReader stream(&file);
  while ( ! stream.atEnd())
  {
    QXmlStreamReader::TokenType token = stream.readNext();
    switch (token)
    {
      case QXmlStreamReader::StartElement:

        // check if workspace element
        if (KWorkspaceTag == stream.name())
        {
          // check file version
          const int majorVersion    = stream.attributes().value(KMajorVersionAttribute).toInt();
          const int minorVersion    = stream.attributes().value(KMinorVersionAttribute).toInt();
          const int revisionVersion = stream.attributes().value(KRevisionVersionAttribute).toInt();

          if ((KMajorVersion != majorVersion) || (KMinorVersion != minorVersion) || (KRevisionVersion != revisionVersion))
          {
            // error!
            QMessageBox::warning(this, tr("Open Project error"), tr("Invalid project version!"), QMessageBox::Ok);
            return;
          }
        }
        else
        {
          // allow others to process element
          emit loadData(stream);
        }
        break;
    }
  }

  // get project (ie check if was created successfully)
  Project* project = ObjectPool::Instance()->getObject<Project>();
  if ((NULL == project) || stream.hasError())
  {
    // error!
    QMessageBox::warning(this, tr("Open Project error"), tr("Could not open project file!"), QMessageBox::Ok);
    closeProject();
  }
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

  Project* project = ObjectPool::Instance()->getObject<Project>();
  Q_ASSERT(project);

  // save workspace data first
  QString output;
  QXmlStreamWriter stream(&output);
  stream.setAutoFormatting(true);
  stream.writeStartDocument();
  stream.writeStartElement("workspace");
  stream.writeAttribute("version-major", QString("%1").arg(KMajorVersion));
  stream.writeAttribute("version-minor", QString("%1").arg(KMinorVersion));
  stream.writeAttribute("version-revision", QString("%1").arg(KRevisionVersion));

  // emit signal so other components can save data as well
  emit saveData(stream);

  // finalize workspace data block
  stream.writeEndElement();
  stream.writeEndDocument();

  // check save status
  bool result = ! stream.hasError();
  if (result)
  {
    // save it to file
    QFile file(FileSystemUtils::Join(project->path(), project->name() + "." + KProjectFileExtension));
    result = file.open(QIODevice::WriteOnly | QIODevice::Text);
    if (result)
    {
      QTextStream out(&file);
      out << output;
    }
  }

  // check for errors
  if ( ! result)
  {
    // error!
    QMessageBox::critical(this, tr("Save Error!"), tr("Error occured during the save process!"));
  }
  else
  {
    // validate project
    project->setDirty(false);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void MainWindow::onObjectAdded(QObject* object)
{
  // check if project added
  if (NULL != qobject_cast<Project*>(object))
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
  m_ui->mdiArea->addSubWindow(widget);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void MainWindow::removeChildWindow(QWidget* widget)
{
  m_ui->mdiArea->removeSubWindow(widget);
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
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void MainWindow::saveSettings()
{
  Settings* settings = ObjectPool::Instance()->getObject<Settings>();
  Q_ASSERT(NULL != settings);

  if (NULL != settings)
  {
    settings->beginGroup("main-window");
    settings->setValue("geometry", saveGeometry());
    settings->setValue("state", saveState());
    settings->endGroup();

    // sync
    settings->sync();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void MainWindow::loadSettings()
{
  Settings* settings = ObjectPool::Instance()->getObject<Settings>();
  Q_ASSERT(NULL != settings);

  if (NULL != settings)
  {
    settings->beginGroup("main-window");
    restoreGeometry(settings->value("geometry").toByteArray());
    restoreState(settings->value("state").toByteArray());
    settings->endGroup();

    // sync
    settings->sync();
  }
}
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
    case EFile:           menuName = "fileMenu"; break;
    case EView:           menuName = "viewMenu"; break;
    case EActions:        menuName = "actionsMenu"; break;
    case EProject:        menuName = "projectMenu"; break;
    case EProjectModules: menuName = "menuModules"; break;

    default:
      break;
  }

  QMenu* menuObject = menuBar()->findChild<QMenu*>(menuName);
  Q_ASSERT(menuObject);

  return menuObject;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
