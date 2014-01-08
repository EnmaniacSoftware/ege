#include "ResourceLibrary.h"
#include "ResourceLibraryDataModel.h"
#include "ResourceLibraryDataModelProxy.h"
#include "ResourceItem.h"
#include "ResourceLibraryWindow.h"
#include <PropertiesWindow.h>
#include <Projects/Project.h>
#include <Configuration.h>
#include <ObjectPool.h>
#include <QList>
#include <QDebug>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const QString KResourceLibraryTag = "ResourceLibrary";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceLibrary::ResourceLibrary(ResourceLibraryWindow* window, QObject* parent) : QObject(parent),
                                                                                   m_model(new ResourceLibraryDataModel(this)),
                                                                                   m_filterProxy(new ResourceLibraryDataModelProxy(this))
{
  // bind original model to proxy
  m_filterProxy->setSourceModel(m_model);

  // set view model
  window->view()->setModel(m_filterProxy);

  // connect
  // NOTE: make sure onModelChanged is connected AFTER source model is attached to proxy so proxy is notified first about the changes
  connect(ObjectPool::Instance(), SIGNAL(objectAdded(QObject*)), this, SLOT(onObjectAdded(QObject*)));
  connect(ObjectPool::Instance(), SIGNAL(objectRemoved(QObject*)), this, SLOT(onObjectRemoved(QObject*)));
  connect(m_model, SIGNAL(rowsInserted(const QModelIndex&, int, int)), window, SLOT(onModelChanged()));
  connect(m_model, SIGNAL(itemChanged(const ResourceItem*)), this, SLOT(onItemChanged(const ResourceItem*)));
  connect(m_filterProxy, SIGNAL(filterChanged()), window, SLOT(onModelChanged()));
  connect(window->view()->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
          this, SLOT(onSelectionChanged(QItemSelection, QItemSelection)));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceLibrary::~ResourceLibrary()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceLibrary::onSaveData(QXmlStreamWriter& stream)
{
  // store data
  stream.writeStartElement(KResourceLibraryTag);

  // serialize model
  model()->serialize(stream);

  // finalize
  stream.writeEndElement();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceLibrary::onLoadData(QXmlStreamReader& stream)
{
  // check if proper element
  if (KResourceLibraryTag == stream.name())
  {
    // unserialize model
    model()->unserialize(stream);

    // emit
    emit loaded(model()->rowCount());
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceLibraryDataModel* ResourceLibrary::model() const
{
  return m_model;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceLibrary::insertItem(ResourceItem* item)
{
  Q_ASSERT(NULL != item);

  ResourceLibraryWindow* window = ObjectPool::Instance()->getObject<ResourceLibraryWindow>();
  Q_ASSERT(NULL != window);

  // get current selection index
  QModelIndexList list = window->selectedIndexes();
  QModelIndex index = ! list.isEmpty() ? list.front() : QModelIndex();

  // add item into model
  model()->insertItem(index, item);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceLibrary::removeSelectedItems()
{
  ResourceLibraryWindow* window = ObjectPool::Instance()->getObject<ResourceLibraryWindow>();
  Q_ASSERT(NULL != window);

  QModelIndexList indexList = window->selectedIndexes();
  foreach (const QModelIndex& index, indexList)
  {
    // remove in source model
    model()->removeItem(index);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ResourceLibrary::isEmpty() const
{
  qDebug() << "Row count:" << proxyModel() << proxyModel()->rowCount() << model() << model()->rowCount();

  return (0 == proxyModel()->rowCount());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QList<ResourceItem*> ResourceLibrary::items(const QString& typeName) const
{
  Configuration* configuration = ObjectPool::Instance()->getObject<Configuration>();
  Q_ASSERT(NULL != configuration);

  // get all items of a given type from model
  QList<ResourceItem*> list = model()->items(typeName);

  // leave only ones belonging to current configuration
  for (QList<ResourceItem*>::iterator it = list.begin(); it != list.end();)
  {
    const ResourceItem* item = *it;

    // check if invalid type
    if (item->configurationName() != configuration->current())
    {
      // remove it
      it = list.erase(it);
    }
    else
    {
      // go on
      ++it;
    }
  }

  return list;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceLibrary::onObjectAdded(QObject* object)
{
  // check if project added
  Project* project = qobject_cast<Project*>(object);
  if (NULL != project)
  {
    connect(model(), SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), project, SLOT(onProjectDataChanged()));
    connect(model(), SIGNAL(rowsInserted(const QModelIndex&, int, int)), project, SLOT(onProjectDataChanged()));
    connect(model(), SIGNAL(rowsMoved(const QModelIndex&, int, int, const QModelIndex&, int)), project, SLOT(onProjectDataChanged()));
    connect(model(), SIGNAL(rowsRemoved(const QModelIndex&, int, int)), project, SLOT(onProjectDataChanged()));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceLibrary::onObjectRemoved(QObject* object)
{
  // check if project removed
  if (qobject_cast<Project*>(object))
  {
    // clean up model
    model()->clear();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceLibraryDataModelProxy* ResourceLibrary::proxyModel() const
{
  return m_filterProxy;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceLibrary::onSelectionChanged(const QItemSelection& selectedItems, const QItemSelection& deselectedItems)
{
  PropertiesWindow* window = ObjectPool::Instance()->getObject<PropertiesWindow>();
  Q_ASSERT(NULL != window);

  QModelIndexList indexList = selectedItems.indexes();
  foreach (const QModelIndex& index, indexList)
  {
    const ResourceLibraryDataModelProxy* proxyModel = qobject_cast<const ResourceLibraryDataModelProxy*>(index.model());

    // check if index is part of any proxy model
    if (NULL != proxyModel)
    {
      // convert index to source
      QModelIndex sourceIndex = proxyModel->mapToSource(index);

      ResourceItem* selectedItem = static_cast<ResourceItem*>(sourceIndex.internalPointer());
      window->attach(selectedItem);
    }
  }

  if (indexList.isEmpty())
  {
    // detach
    window->attach(NULL);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceLibrary::onItemChanged(const ResourceItem* item)
{
  Q_UNUSED(item);

  ResourceLibraryWindow* window = ObjectPool::Instance()->getObject<ResourceLibraryWindow>();
  Q_ASSERT(NULL != window);

  // get current selection index
  QModelIndexList list = window->selectedIndexes();
  QModelIndex index = ! list.isEmpty() ? list.front() : QModelIndex();

  // propagate
  emit model()->dataChanged(index, index);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
