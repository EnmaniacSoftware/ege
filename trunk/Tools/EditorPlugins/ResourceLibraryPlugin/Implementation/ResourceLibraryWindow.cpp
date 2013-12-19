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
ResourceLibraryWindow::ResourceLibraryWindow(ResourceLibrary* library, QWidget* parent) : QDockWidget(parent),
                                                                                          m_ui(new Ui_ResourceLibrary()),
                                                                                          m_resourceLibrary(library)
{
  // setup UI
  m_ui->setupUi(this);

  // set view model
  m_ui->view->setModel(library->model());

  // connect
  connect(ObjectPool::Instance(), SIGNAL(objectAdded(QObject*)), this, SLOT(onObjectAdded(QObject*)));
  connect(ObjectPool::Instance(), SIGNAL(objectRemoved(QObject*)), this, SLOT(onObjectRemoved(QObject*)));

  connect(m_resourceLibrary->model(), SIGNAL(rowsInserted(const QModelIndex&, int, int)), this, SLOT(onModelChanged()));

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
  ResourceItemFactory* factory = ObjectPool::Instance()->getObject<ResourceItemFactory>();
  Q_ASSERT(NULL != factory);

  ResourceItem* selectedItem = NULL;

  QMenu menu(this);

  // get selected indicies
  QModelIndexList indexList = m_ui->view->selectionModel()->selectedIndexes();

  // check if anything selected
  if ( ! indexList.empty())
  {
    // find selected item
    QModelIndex modelIndex = indexList.front();
    selectedItem = static_cast<ResourceItem*>(modelIndex.internalPointer());
  }

  // let registered items hook up to context menu
  const QList<ResourceItemFactory::ItemData>& registeredItems = factory->items();
  foreach (const ResourceItemFactory::ItemData& item, registeredItems)
  {
    item.resourceLibraryWindowHookFunc(menu, (NULL != selectedItem) ? selectedItem->type() : QString());
  }

  //action = menu.addAction(tr("Remove"), this, SLOT(onRemoveItems()));

/*
  if (0 == indexList.size())
  {
    action = menu.addAction(tr("Add group"), this, SLOT(onAddGroup()));
  }
  else if (1 == indexList.size())
  {
    QModelIndex modelIndex = indexList.front();
    ResourceItem* selectedItem = static_cast<ResourceItem*>(modelIndex.internalPointer());

    // let registered items hook up to context menu
    const QList<ResourceItemFactory::ItemData>& registeredItems = factory->items();
    foreach (const ResourceItemFactory::ItemData& item, registeredItems)
    {
      item.resourceLibraryWindowHookFunc(menu, selectedItem->type());
    }

    action = menu.addAction(tr("Remove"), this, SLOT(onRemoveItems()));
  }
  else
  {
    action = menu.addAction(tr("Remove"), this, SLOT(onRemoveItems()));
  }*/

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
    // set view delegate
    ResourceLibraryItemDelegate* delegate = static_cast<ResourceLibraryItemDelegate*>(project->resourceLibraryItemDelegate());
    delegate->setView(m_ui->view);

    m_ui->view->setItemDelegate(delegate);

    // establish connections
    connect(m_ui->stackedWidget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(onQueueContextMenuRequested(const QPoint&)));

    connect(m_resourceLibrary->model(), SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), project, SLOT(onProjectDataChanged()));
    connect(m_resourceLibrary->model(), SIGNAL(rowsInserted(const QModelIndex&, int, int)), project, SLOT(onProjectDataChanged()));
    connect(m_resourceLibrary->model(), SIGNAL(rowsMoved(const QModelIndex&, int, int, const QModelIndex&, int)), project, SLOT(onProjectDataChanged()));
    connect(m_resourceLibrary->model(), SIGNAL(rowsRemoved(const QModelIndex&, int, int)), project, SLOT(onProjectDataChanged()));

    // set resource page
    m_ui->stackedWidget->setCurrentIndex(KStackedPageIndexAddResources);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceLibraryWindow::onObjectRemoved(QObject* object)
{
  // check if project removed
  if (qobject_cast<Project*>(object))
  {
    // reset view delegate
    m_ui->view->setItemDelegate(NULL);

    // clean up model
    m_resourceLibrary->model()->clear();

    // make disconnections
    disconnect(m_ui->stackedWidget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(onQueueContextMenuRequested(const QPoint&)));

    // force model change notification
    // NOTE: this is required for proper stack widget update if model was previously empty
    onModelChanged();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceLibraryWindow::onLibraryLoaded(int count)
{
  Q_UNUSED(count);
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
  QModelIndexList indexList = m_ui->view->selectionModel()->selectedIndexes();
  foreach (const QModelIndex& index, indexList)
  {
    m_resourceLibrary->model()->removeItem(index);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceLibraryWindow::updateMenus()
{
  MainWindow* mainWindow = ObjectPool::Instance()->getObject<MainWindow>();
  Q_ASSERT(NULL != mainWindow);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QModelIndexList ResourceLibraryWindow::selectedIndexes() const
{
  return m_ui->view->selectionModel()->selectedIndexes();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceLibraryWindow::onModelChanged()
{
  Project* project = ObjectPool::Instance()->getObject<Project>();

  // update stack widget page
  // Possible cases are:
  // - No project -> resources are NOT available.
  // - Project available, model is empty -> add more resources.
  // - Project available, model is NOT empty -> view resources.
  int stackedPageToSet = KStackedPageIndexResourcesNotAvailable;

  if (NULL != project)
  {
    stackedPageToSet = m_resourceLibrary->model()->isEmpty() ? KStackedPageIndexAddResources : KStackedPageIndexResourcesView;
  }

  m_ui->stackedWidget->setCurrentIndex(stackedPageToSet);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
