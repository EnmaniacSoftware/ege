#include "ResourceLibraryWindow.h"
#include "ui_ResourceLibraryWindow.h"
#include "ResourceLibraryDataModel.h"
#include "ResourceLibraryItemDelegate.h"
#include "ResourceItem.h"
#include "ResourceItemFactory.h"
#include "ResourceItemContainer.h"
#include "ResourceLibrary.h"
#include <Projects/Project.h>
#include <MainWindow.h>
#include <ObjectPool.h>
#include <QMenuBar>
#include <QFile>
#include <QFileDialog>
#include <QDebug>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const int KStackedPageIndexResourcesNotAvailable = 0;
static const int KStackedPageIndexAddResources          = 1;
static const int KStackedPageIndexResourcesView         = 2;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceLibraryWindow::ResourceLibraryWindow(QWidget* parent) : QDockWidget(parent),
                                                                m_ui(new Ui_ResourceLibrary())
{
  // setup UI
  m_ui->setupUi(this);
  
  // connect
  connect(ObjectPool::Instance(), SIGNAL(objectAdded(QObject*)), this, SLOT(onObjectAdded(QObject*)));
  connect(ObjectPool::Instance(), SIGNAL(objectRemoved(QObject*)), this, SLOT(onObjectRemoved(QObject*)));
  
  // update menus
  updateMenus();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceLibraryWindow::~ResourceLibraryWindow()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceLibraryWindow::saveSettings()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceLibraryWindow::loadSettings()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceLibraryWindow::onQueueContextMenuRequested(const QPoint& pos)
{
  QMenu menu(this);

  QAction* action = NULL;

  QModelIndexList indexList = m_ui->view->selectionModel()->selectedIndexes();

  if (0 == indexList.size())
  {
    action = menu.addAction(tr("Add group"), this, SLOT(onAddGroup()));
  }
  else if (1 == indexList.size())
  {
    QModelIndex modelIndex = indexList.front();
    ResourceItem* item = static_cast<ResourceItem*>(modelIndex.internalPointer());

    if (ResourceItemContainer::TypeName() == item->type())
    {
      action = menu.addAction(tr("Add resource"), this, SLOT(onAddResource()));
    }

    action = menu.addAction(tr("Remove"), this, SLOT(onRemoveItems()));
  }
  else
  {
    action = menu.addAction(tr("Remove"), this, SLOT(onRemoveItems()));
  }

  if ( ! menu.isEmpty())
  {
    menu.exec(m_ui->view->mapToGlobal(pos));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceLibraryWindow::onObjectAdded(QObject* object)
{
  // check if project added
  Project* project = qobject_cast<Project*>(object);
  if (NULL != project)
  {
    ResourceLibrary* library = ObjectPool::Instance()->getObject<ResourceLibrary>();
    Q_ASSERT(NULL != library);

    // set view delegate
    ResourceLibraryItemDelegate* delegate = static_cast<ResourceLibraryItemDelegate*>(project->resourceLibraryItemDelegate());
    delegate->setView(m_ui->view);

    m_ui->view->setItemDelegate(delegate);

    // establish connections
    connect(m_ui->stackedWidget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(onQueueContextMenuRequested(const QPoint&)));
    connect(library->model(), SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), project, SLOT(onProjectDataChanged()));
    connect(library->model(), SIGNAL(rowsInserted(const QModelIndex&, int, int)), project, SLOT(onProjectDataChanged()));
    connect(library->model(), SIGNAL(rowsMoved(const QModelIndex&, int, int, const QModelIndex&, int)), project, SLOT(onProjectDataChanged()));
    connect(library->model(), SIGNAL(rowsRemoved(const QModelIndex&, int, int)), project, SLOT(onProjectDataChanged()));

    // show resources page
    m_ui->stackedWidget->setCurrentIndex(KStackedPageIndexAddResources);
  }

  // check if resource library added
  ResourceLibrary* library = qobject_cast<ResourceLibrary*>(object);
  if (NULL != library)
  {
    // set view model
    m_ui->view->setModel(library->model());
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceLibraryWindow::onObjectRemoved(QObject* object)
{
  // check if project removed
  if (qobject_cast<Project*>(object))
  {
    ResourceLibrary* library = ObjectPool::Instance()->getObject<ResourceLibrary>();
    Q_ASSERT(NULL != library);

    // reset view delegate
    m_ui->view->setItemDelegate(NULL);

    // clean up model
    library->model()->clear();

    // make disconnections
    disconnect(m_ui->stackedWidget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(onQueueContextMenuRequested(const QPoint&)));

    // hide resources page
    m_ui->stackedWidget->setCurrentIndex(KStackedPageIndexResourcesNotAvailable);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceLibraryWindow::onLibraryLoaded(int count)
{
  // update resources page
  m_ui->stackedWidget->setCurrentIndex((0 == count) ? KStackedPageIndexAddResources : KStackedPageIndexResourcesView);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceLibraryWindow::onAddGroup()
{
  ResourceLibrary* library = ObjectPool::Instance()->getObject<ResourceLibrary>();
  Q_ASSERT(NULL != library);

  // get current seclection index
  QModelIndexList list = m_ui->view->selectionModel()->selectedIndexes();
  QModelIndex index = ! list.isEmpty() ? list.front() : QModelIndex();

  // create container item
  ResourceItemFactory* factory = ObjectPool::Instance()->getObject<ResourceItemFactory>();
  Q_ASSERT(NULL != factory);

  if (NULL != factory)
  {
    ResourceItem* newItem = factory->createItem(ResourceItemContainer::TypeName(), generateGroupName());
    if (NULL != newItem)
    {
      library->model()->insertItem(index, newItem);

      // show resources view
      m_ui->stackedWidget->setCurrentIndex(KStackedPageIndexResourcesView);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceLibraryWindow::onAddResource()
{
  ResourceLibrary* library = ObjectPool::Instance()->getObject<ResourceLibrary>();
  Q_ASSERT(NULL != library);

  QModelIndex index = m_ui->view->selectionModel()->selectedIndexes().first();

  // prepare filters
	QString filters = tr("Images");
	filters += QLatin1String(" (*.png *.jpg)");

  // open file selection dialog
	QStringList list = QFileDialog::getOpenFileNames(this, tr("Add resource"), QString(), filters);
  if ( ! list.isEmpty())
  {
    // create container item
    ResourceItemFactory* factory = ObjectPool::Instance()->getObject<ResourceItemFactory>();
    Q_ASSERT(NULL != factory);

    if (NULL != factory)
    {
      // go thru all items
      for (int i = 0; i < list.size(); ++i)
      {
        QString item = QDir::fromNativeSeparators(list[i]);
        QString name = item.section("/", -1);
        QString path = item.section("/", 0, -2);

        ResourceItem* newItem = factory->createItem("image", name);
        if (NULL != newItem)
        {
          QModelIndex childIndex = library->model()->insertItem(index, newItem);

          library->model()->setData(childIndex, QVariant(path), ResourceLibraryDataModel::PathRole);
        }
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceLibraryWindow::onRemoveItems()
{
  ResourceLibrary* library = ObjectPool::Instance()->getObject<ResourceLibrary>();
  Q_ASSERT(NULL != library);

  QModelIndexList indexList = m_ui->view->selectionModel()->selectedIndexes();
  foreach (const QModelIndex& index, indexList)
  {
    library->model()->removeItem(index);
  }

  // check if no resources available
  if (library->model()->isEmpty())
  {
    // change stacked page
    m_ui->stackedWidget->setCurrentIndex(KStackedPageIndexAddResources);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceLibraryWindow::updateMenus()
{
  MainWindow* mainWindow = ObjectPool::Instance()->getObject<MainWindow>();
  Q_ASSERT(NULL != mainWindow);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QString ResourceLibraryWindow::generateGroupName() const
{
  ResourceLibrary* library = ObjectPool::Instance()->getObject<ResourceLibrary>();
  Q_ASSERT(NULL != library);
  Q_ASSERT(NULL != library->model());

  QString name;

  // get all present groups
  QList<ResourceItem*> availableGroups = library->model()->items(ResourceItemContainer::TypeName());

  int index = 1;
  while (true)
  {
    // generate temporary name
    name = tr("Group %1").arg(index++);

    // check if does not exist
    bool found = false;
    foreach (const ResourceItem* item, availableGroups)
    {
      // check if found
      if (item->name() == name)
      {
        // done
        found = true;
        break;
      }
    }

    // check if not found
    if ( ! found)
    {
      // we are done
      break;
    }
  }

  return name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
