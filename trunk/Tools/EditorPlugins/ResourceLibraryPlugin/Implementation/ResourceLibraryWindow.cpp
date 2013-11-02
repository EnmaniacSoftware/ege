#include "ResourceLibraryWindow.h"
#include "ui_ResourceLibraryWindow.h"
#include "ResourceLibraryDataModel.h"
#include "ResourceLibraryItemDelegate.h"
#include "ResourceItem.h"
#include "ResourceItemFactory.h"
#include <Projects/Project.h>
#include <MainWindow.h>
#include <ObjectPool.h>
#include <QMenuBar>
#include <QFile>
#include <QFileDialog>
#include <QDebug>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceLibraryWindow::ResourceLibraryWindow(QWidget* parent) : QDockWidget(parent),
                                                                m_ui(new Ui_ResourceLibrary()),
                                                                m_model(new ResourceLibraryDataModel(this))
{
  // setup UI
  m_ui->setupUi(this);
  
  // connect
  connect(ObjectPool::Instance(), SIGNAL(objectAdded(QObject*)), this, SLOT(onObjectAdded(QObject*)));
  connect(ObjectPool::Instance(), SIGNAL(objectRemoved(QObject*)), this, SLOT(onObjectRemoved(QObject*)));

  // set view model
  m_ui->view->setModel(m_model);
  
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
    action = menu.addAction(tr("Add container"), this, SLOT(onAddContainer()));
  }
  else if (1 == indexList.size())
  {
    QModelIndex modelIndex = indexList.front();
    ResourceItem* item = static_cast<ResourceItem*>(modelIndex.internalPointer());

    if ("container" == item->type())
    {
      action = menu.addAction(tr("Add container"), this, SLOT(onAddContainer()));
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
    // set view delegate
    ResourceLibraryItemDelegate* delegate = static_cast<ResourceLibraryItemDelegate*>(project->resourceLibraryItemDelegate());
    delegate->setView(m_ui->view);

    m_ui->view->setItemDelegate(delegate);

    // establish connections
    connect(m_ui->stackedWidget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(onQueueContextMenuRequested(const QPoint&)));
    connect(m_model, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), project, SLOT(onProjectDataChanged()));
    connect(m_model, SIGNAL(rowsInserted(const QModelIndex&, int, int)), project, SLOT(onProjectDataChanged()));
    connect(m_model, SIGNAL(rowsMoved(const QModelIndex&, int, int, const QModelIndex&, int)), project, SLOT(onProjectDataChanged()));
    connect(m_model, SIGNAL(rowsRemoved(const QModelIndex&, int, int)), project, SLOT(onProjectDataChanged()));

    // show resources page
    m_ui->stackedWidget->setCurrentIndex(1);
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
    m_model->clear();

    // make disconnections
    disconnect(m_ui->stackedWidget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(onQueueContextMenuRequested(const QPoint&)));

    // hide resources page
    m_ui->stackedWidget->setCurrentIndex(0);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceLibraryWindow::onAddContainer()
{
  ResourceItem* item;

  // get current seclection index
  QModelIndexList list = m_ui->view->selectionModel()->selectedIndexes();
  QModelIndex index = ! list.isEmpty() ? list.front() : QModelIndex();

  ResourceItem* newItem = NULL;//app->resourceItemFactory()->createItem("container", tr("No name"));
  if (newItem)
  {
    m_model->insertItem(index, newItem);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceLibraryWindow::onAddResource()
{
  QModelIndex index = m_ui->view->selectionModel()->selectedIndexes().first();

  // prepare filters
	QString filters = tr("Images");
	filters += QLatin1String(" (*.png *.jpg)");

  // open file selection dialog
	QStringList list = QFileDialog::getOpenFileNames(this, tr("Add resource"), QString(), filters);
  if ( !list.isEmpty())
  {
    // go thru all items
    for (int i = 0; i < list.size(); ++i)
    {
      QString item = QDir::fromNativeSeparators(list[i]);
      QString name = item.section("/", -1);
      QString path = item.section("/", 0, -2);

      ResourceItem* newItem = NULL;//app->resourceItemFactory()->createItem("image", name);
      if (newItem)
      {
        QModelIndex childIndex = m_model->insertItem(index, newItem);

        m_model->setData(childIndex, QVariant(path), ResourceLibraryDataModel::PathRole);
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
    m_model->removeItem(index);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ResourceLibraryWindow::serialize(QXmlStreamWriter& stream) const
{
  stream.writeStartElement("resources");
  
  // serialize model
  bool result = m_model->serialize(stream);

  stream.writeEndElement();

  return result && !stream.hasError();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ResourceLibraryWindow::unserialize(QXmlStreamReader& stream)
{
  // unserialize model
  bool result = m_model->unserialize(stream);
 
  return result && !stream.hasError();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceLibraryWindow::updateMenus()
{
  MainWindow* mainWindow = ObjectPool::Instance()->getObject<MainWindow>();
  if (!mainWindow)
  {
    // error!
    qWarning() << Q_FUNC_INFO << "No MainWindow found!";
    return;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceLibraryDataModel* ResourceLibraryWindow::model() const
{
  return m_model;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
