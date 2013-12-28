#include "ResourceLibrary.h"
#include "ResourceLibraryDataModel.h"
#include "ResourceItem.h"
#include "ResourceLibraryWindow.h"
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
                                                                                   m_filterProxy(new QSortFilterProxyModel(this))
{
  Configuration* configuration = ObjectPool::Instance()->getObject<Configuration>();
  Q_ASSERT(NULL != configuration);

  // bind original model to proxy
  m_filterProxy->setSourceModel(m_model);

  // set view model
  window->view()->setModel(m_filterProxy);

  // connect
  // NOTE: make sure onModelChanged is connected AFTER source model is attached to proxy so proxy is notified first about the changes
  connect(ObjectPool::Instance(), SIGNAL(objectAdded(QObject*)), this, SLOT(onObjectAdded(QObject*)));
  connect(ObjectPool::Instance(), SIGNAL(objectRemoved(QObject*)), this, SLOT(onObjectRemoved(QObject*)));
  connect(configuration, SIGNAL(changed(QString)), this, SLOT(onConfigurationChanged(QString)));
  connect(m_model, SIGNAL(rowsInserted(const QModelIndex&, int, int)), window, SLOT(onModelChanged()));
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
void ResourceLibrary::onConfigurationChanged(const QString& name)
{

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
  qDebug() << "PROXY ROW COUNT" << proxyModel()->rowCount();

  return (0 == proxyModel()->rowCount());
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
QAbstractProxyModel* ResourceLibrary::proxyModel() const
{
  return m_filterProxy;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
