#include "ResourceLibrary.h"
#include "ui_resourcelibrary.h"
#include "Config.h"
#include "MainWindow.h"
#include "ResourceLibraryDataModel.h"
#include "ResourceLibraryItemDelegate.h"
#include <QMenu>
#include <QFile>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceLibrary::ResourceLibrary(QWidget* parent) : QDockWidget(parent),
                                                    m_ui(new Ui_ResourceLibrary()),
                                                    m_model(new ResourceLibraryDataModel(this)),
                                                    m_itemDelegate(new ResourceLibraryItemDelegate(this))
{
  // setup UI
  m_ui->setupUi(this);

  // set view delegate
  m_ui->view->setItemDelegate(m_itemDelegate);

  // establish connections
	connect(m_ui->stackedWidget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(onQueueContextMenuRequested(const QPoint&)));

  connect(parent, SIGNAL(projectCreated()), this, SLOT(onProjectCreated()));
	connect(parent, SIGNAL(projectClosed()), this, SLOT(onProjectClosed()));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceLibrary::~ResourceLibrary()
{
  if (m_ui)
  {
    delete m_ui;
    m_ui = NULL;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Saves settings. */
void ResourceLibrary::saveSettings(Config* config)
{
  Q_ASSERT(config);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Loads settings. */
void ResourceLibrary::loadSettings(Config* config)
{
  Q_ASSERT(config);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when context menu is requetsed. */
void ResourceLibrary::onQueueContextMenuRequested(const QPoint& pos)
{
  QMenu menu(this);

  QAction* action = NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when new project has been created/opened. */
void ResourceLibrary::onProjectCreated()
{
  // load up data if project file exists
  if (QFile::exists(mainWindow()->project()->fullPath()))
  {
    // load resource library
    if (!m_model->load(mainWindow()->project()->fullPath()))
    {
      // TAGE - error
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when project has been closed. */
void ResourceLibrary::onProjectClosed()
{
  m_model->clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
