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
  
  // set view model
  m_ui->view->setModel(m_model);

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
/*! Slot called when context menu is requested. */
void ResourceLibrary::onQueueContextMenuRequested(const QPoint& pos)
{
  QMenu menu(this);

  QAction* action = NULL;

  QModelIndexList indexList = m_ui->view->selectionModel()->selectedIndexes();

  if (0 == indexList.size())
  {
    action = menu.addAction(tr("Add container"));
  }
  else if (1 == indexList.size())
  {
  }
  else
  {
  }

	if (!menu.isEmpty())
  {
		menu.exec(m_ui->view->mapToGlobal(pos));
  }
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
  else
  {
    // create empty
    m_model->createDefault();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when project has been closed. */
void ResourceLibrary::onProjectClosed()
{
  m_model->clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
