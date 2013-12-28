#include "ResourceLibraryWindow.h"
#include "ResourceLibraryDataModel.h"
#include "ResourceLibraryDataModelProxy.h"
#include "ResourceLibraryItemDelegate.h"
#include "ResourceItem.h"
#include "ResourceItemFactory.h"
#include "ResourceItemGroup.h"
#include "ResourceLibrary.h"
#include "ui_ResourceLibraryWindow.h"
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
  ResourceItemFactory* factory = ObjectPool::Instance()->getObject<ResourceItemFactory>();
  Q_ASSERT(NULL != factory);

  ResourceItem* selectedItem = NULL;

  QMenu menu(this);

  // get selected indicies
  QModelIndexList indexList = selectedIndexes();

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
    item.resourceLibraryWindowHookFunc(menu, (NULL != selectedItem) ? selectedItem->typeName() : QString());
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
    // set view delegate
    ResourceLibraryItemDelegate* delegate = static_cast<ResourceLibraryItemDelegate*>(project->resourceLibraryItemDelegate());
    delegate->setView(m_ui->view);

    m_ui->view->setItemDelegate(delegate);

    // establish connections
    connect(m_ui->stackedWidget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(onQueueContextMenuRequested(const QPoint&)));

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
void ResourceLibraryWindow::onRemoveItems()
{
  // TAGE - implement
  //m_resourceLibrary->removeSelectedItems();
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
  // get selected indicies
  // NOTE: this may return indicies in proxy model or non-proxied model
  QModelIndexList list = m_ui->view->selectionModel()->selectedIndexes();

  // make sure indicies are of original model
  for (int i = 0; i < list.count(); ++i)
  {
    QModelIndex& index = list[i];

    const ResourceLibraryDataModelProxy* proxyModel = qobject_cast<const ResourceLibraryDataModelProxy*>(index.model());

    // check if index is part of any proxy model
    if (NULL != proxyModel)
    {
      // convert index to source
      index = proxyModel->mapToSource(index);
    }
  }

  return list;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QAbstractItemView*ResourceLibraryWindow::view() const
{
  return m_ui->view;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceLibraryWindow::onModelChanged()
{
  ResourceLibrary* library = ObjectPool::Instance()->getObject<ResourceLibrary>();
  Q_ASSERT(NULL != library);

  Project* project = ObjectPool::Instance()->getObject<Project>();

  // update stack widget page
  // Possible cases are:
  // - No project -> resources are NOT available.
  // - Project available, model is empty -> add more resources.
  // - Project available, model is NOT empty -> view resources.
  int stackedPageToSet = KStackedPageIndexResourcesNotAvailable;

  if (NULL != project)
  {
    stackedPageToSet = library->isEmpty() ? KStackedPageIndexAddResources : KStackedPageIndexResourcesView;
  }

  m_ui->stackedWidget->setCurrentIndex(stackedPageToSet);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
