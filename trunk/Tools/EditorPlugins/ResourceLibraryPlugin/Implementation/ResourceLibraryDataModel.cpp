#include "ResourceLibraryDataModel.h"
#include "ResourceItem.h"
#include "ResourceItemFactory.h"
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QStack>
#include <QDebug>
#include <ObjectPool.h>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
// TAGE - duplicate in ResourceItem :/
static const QString KResourceItemTag = "ResourceItem";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceLibraryDataModel::ResourceLibraryDataModel(QObject* parent) : QAbstractItemModel(parent),
                                                                      m_root(new ResourceItem())
{
  m_root->setName("root");
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceLibraryDataModel::~ResourceLibraryDataModel()
{
  clear();

  if (NULL != m_root)
  {
    delete m_root;
    m_root = NULL;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QModelIndex ResourceLibraryDataModel::parent(const QModelIndex& index) const
{
  if ( ! index.isValid())
  {
    return QModelIndex();
  }

  ResourceItem* childItem  = static_cast<ResourceItem*>(index.internalPointer());
  ResourceItem* parentItem = (NULL != childItem) ? childItem->parent() : NULL;

  if ((parentItem == m_root) || (NULL == parentItem))
  {
    return QModelIndex();
  }

  return createIndex(parentItem->row(), 0, parentItem);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QModelIndex ResourceLibraryDataModel::index(int row, int column, const QModelIndex& parent) const
{
  ResourceItem* parentItem;
  ResourceItem* item;

  if ( ! hasIndex(row, column, parent))
  {
    return QModelIndex();
  }

  // check if parent invalid
  if ( ! parent.isValid())
  {
    parentItem = m_root;
  }
  else
  {
    parentItem = static_cast<ResourceItem*>(parent.internalPointer());
  }

  // get proper child
  item = parentItem->child(row);
  Q_ASSERT(NULL != item);

  return createIndex(row, column, item);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QVariant ResourceLibraryDataModel::data(const QModelIndex& index, int role) const
{
  // check if invalid index
  if ( ! index.isValid())
  {
    // done
    return QVariant();
  }

  // get item form model
  ResourceItem* item = getItem(index);
  Q_ASSERT(NULL != item);

  return item->data(index.column(), role);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
int ResourceLibraryDataModel::columnCount(const QModelIndex& parent) const
{
  if (parent.isValid())
  {
    return static_cast<ResourceItem*>(parent.internalPointer())->columnCount();
  }
  
  return m_root->columnCount();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
int ResourceLibraryDataModel::rowCount(const QModelIndex& parent) const
{
  ResourceItem* parentItem = NULL;

  if ( ! parent.isValid())
  {
    parentItem = m_root;
  }
  else
  {
    Q_ASSERT(0 == parent.column());
    parentItem = static_cast<ResourceItem*>(parent.internalPointer());
  }

  // get number of children/rows
  int rowCount = parentItem->childCount();

 //qDebug() << Q_FUNC_INFO << "count" << rowCount << "for" << parent;

  return rowCount;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QList<ResourceItem*> ResourceLibraryDataModel::items(const QString& typeName) const
{
  QList<ResourceItem*> list = allItems();

  // go thru the list and remove invalid types
  for (QList<ResourceItem*>::iterator it = list.begin(); it != list.end();)
  {
    const ResourceItem* item = *it;

    // check if invalid type
    if (item->typeName() != typeName)
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
void ResourceLibraryDataModel::clear()
{
  if ((NULL != m_root) && (0 < m_root->childCount()))
  {
	  beginRemoveRows(QModelIndex(), 0, m_root->childCount() - 1);
    m_root->removeChildren();
	  endRemoveRows();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Qt::ItemFlags ResourceLibraryDataModel::flags(const QModelIndex& index) const
{
  if ( ! index.isValid())
  {
    return 0;
  }

  ResourceItem* item = getItem(index);
  Q_ASSERT(NULL != item);

  return item->flags();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceItem* ResourceLibraryDataModel::getItem(const QModelIndex& index) const
{
  if (index.isValid()) 
  {
    ResourceItem* item = static_cast<ResourceItem*>(index.internalPointer());
    return item;
  }

  return m_root;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ResourceLibraryDataModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
  ResourceItem* item = getItem(index);

  // process according to role
  bool success = item->setData(value, role);

  // emit
  if (success)
  {
    emit dataChanged(index, index);
  }

  return success;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ResourceLibraryDataModel::serialize(QXmlStreamWriter& stream) const
{
  bool result = true;

  // serialize root's items (without root itself)
  const int children = m_root->childCount();
  for (int i = 0; (i < children) && result; ++i)
  {
    const ResourceItem* child = m_root->child(i);
    Q_ASSERT(NULL != child);

    // serialize current child
    result = child->serialize(stream);
  }

  return result && ! stream.hasError();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ResourceLibraryDataModel::unserialize(QXmlStreamReader& stream)
{
  ResourceItemFactory* factory = ObjectPool::Instance()->getObject<ResourceItemFactory>();
  Q_ASSERT(NULL != factory);

  // NOTE: ResourceItem stack is used to track the parents of deserialized items
  QStack<ResourceItem*> itemStack;
  itemStack.push_back(m_root);

  while ( ! stream.atEnd())
  {
    QXmlStreamReader::TokenType token = stream.readNext();
    switch (token)
    {
      case QXmlStreamReader::StartElement:

        // check if resource item tag
        if (KResourceItemTag == stream.name())
        {
          // get resource item required data
          const QString type = stream.attributes().value("type").toString();
          const QString name = stream.attributes().value("name").toString();

          // create given resource item
          ResourceItem* item = factory->createItem(type, name, itemStack.top());
          Q_ASSERT(NULL != item);

          // unserialize item
          if (item->unserialize(stream))
          {
            // add to parent
            itemStack.top()->addChild(item);

            // add this item to local stack
            itemStack.push_back(item);
          }
          else
          {
            // error!
            qWarning() << tr("Could not unserialize resource item of name:") << name << tr("and type:") << type;
          }
        }
        break;

      case QXmlStreamReader::EndElement:

        // check if resource item tag
        if (KResourceItemTag == stream.name())
        {
          // pop from local item stack
          itemStack.pop_back();
        }
        break;
    }
  }

  return ! stream.hasError();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceLibraryDataModel::removeItem(const QModelIndex& index)
{
  // get item form model
  ResourceItem* item = getItem(index);
  Q_ASSERT((NULL != item) && item->parent());

  beginRemoveRows(index.parent(), item->row(), item->row());
  item->parent()->removeChild(item);
	endRemoveRows();

  emit itemRemoved(item);
  delete item;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QModelIndex ResourceLibraryDataModel::insertItem(const QModelIndex& index, ResourceItem* item)
{
  // get item form model
  ResourceItem* parentItem = getItem(index);

  // add to the end of the pool
  beginInsertRows(index.parent(), parentItem->childCount(), parentItem->childCount());
  parentItem->addChild(item);
  endInsertRows();

  emit itemAdded(item);

  return createIndex(parentItem->childCount() - 1, 0, item);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QList<ResourceItem*> ResourceLibraryDataModel::allItems() const
{
  QList<ResourceItem*> list;

  addChildren(m_root, list);

  return list;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceLibraryDataModel::addChildren(ResourceItem* item, QList<ResourceItem*>& list) const
{
  for (int i = 0; i < item->childCount(); ++i)
  {
    ResourceItem* child = item->child(i);

    // add current child children first
    addChildren(child, list);

    // add child
    list.append(child);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceItem* ResourceLibraryDataModel::root() const
{
  return m_root;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ResourceLibraryDataModel::isEmpty() const
{
  return (0 == m_root->childCount());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
