#include "ResourceLibraryDataModel.h"
#include "Resources/ResourceItem.h"
#include "Resources/ResourceItemFactory.h"
#include "MainWindow.h"
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QDebug>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceLibraryDataModel::ResourceLibraryDataModel(QObject* parent) : QAbstractItemModel(parent)
{
  m_root = new ResourceItem();
  if (m_root)
  {
    m_root->setName("root");
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceLibraryDataModel::~ResourceLibraryDataModel()
{
  clear();

  if (m_root)
  {
    delete m_root;
    m_root = NULL;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! QAbstractItemModel override. Returns the parent of the model item with the given index. If the item has no parent, an invalid QModelIndex is returned. */
QModelIndex ResourceLibraryDataModel::parent(const QModelIndex& index) const
{
  if (!index.isValid())
  {
    return QModelIndex();
  }

  ResourceItem* childItem = static_cast<ResourceItem*>(index.internalPointer());
  ResourceItem* parentItem = (childItem) ? childItem->parent() : NULL;

  if ((parentItem == m_root) || (NULL == parentItem))
  {
    return QModelIndex();
  }

  return createIndex(parentItem->row(), 0, parentItem);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! QAbstractItemModel override. Returns the index of the item in the model specified by the given row, column and parent index. */
QModelIndex ResourceLibraryDataModel::index(int row, int column, const QModelIndex& parent) const
{
  ResourceItem* parentItem;
  ResourceItem* item;

  // check if parent invalid
  if (!parent.isValid())
  {
    parentItem = m_root;
  }
  else
  {
    parentItem = static_cast<ResourceItem*>(parent.internalPointer());
  }

  // get proper child
  item = parentItem->child(row);

  return createIndex(row, column, item);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! QAbstractItemModel override. Returns the data stored under the given role for the item referred to by the index. */
QVariant ResourceLibraryDataModel::data(const QModelIndex& index, int role) const
{
  // check if invalid index
  if (!index.isValid())
  {
    // done
    return QVariant();
  }

  // get item form model
  ResourceItem* item = getItem(index);
  return item->data(index.column(), role);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! QAbstractItemModel override. Returns the number of columns for the children of the given parent. */
int ResourceLibraryDataModel::columnCount(const QModelIndex& parent) const
{
  if (parent.isValid())
  {
    return static_cast<ResourceItem*>(parent.internalPointer())->columnCount();
  }
  
  return m_root->columnCount();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! QAbstractItemModel override. Returns the number of rows under the given parent. */
int ResourceLibraryDataModel::rowCount(const QModelIndex& parent) const
{
  ResourceItem* parentItem = NULL;

  if (0 < parent.column())
  {
    return 0;
  }

  if (!parent.isValid())
  {
    parentItem = m_root;
  }
  else
  {
    parentItem = static_cast<ResourceItem*>(parent.internalPointer());
  }

  return parentItem->childCount();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Clears data. */
void ResourceLibraryDataModel::clear()
{
  if (m_root && (0 < m_root->childCount()))
  {
	  beginRemoveRows(QModelIndex(), 0, m_root->childCount() - 1);
    m_root->removeChildren();
	  endRemoveRows();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! QAbstractItemModel override. Returns the item flags for the given index. */
Qt::ItemFlags ResourceLibraryDataModel::flags(const QModelIndex& index) const
{
  if (!index.isValid())
  {
    return 0;
  }

  ResourceItem* item = getItem(index);
  return item->flags();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns resource library item connected for given model index. */
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
/*! QAbstractItemModel override. Sets the role data for the item at index to value.*/
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
/*! Serializes into given stream. */
bool ResourceLibraryDataModel::serialize(QXmlStreamWriter& stream) const
{
  // serialize root
  bool result = m_root->serialize(stream);

  return result && !stream.hasError();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Unserializes from given data stream. */
bool ResourceLibraryDataModel::unserialize(QXmlStreamReader& stream)
{
  bool done = false;
  while (!stream.atEnd() && !done)
  {
    QXmlStreamReader::TokenType token = stream.readNext();
    switch (token)
    {
      case QXmlStreamReader::StartElement:

        // check if root element
        if ("resource-item" == stream.name() && stream.attributes().value("type") == m_root->type() && stream.attributes().value("name") == m_root->name())
        {
          if (!m_root->unserialize(stream))
          {
            // error!
            return false;
          }
        }
        else
        {
          qWarning() << "Skipping data: " << stream.name();
        }
        break;

      case QXmlStreamReader::EndElement:

        // done
        done = true;
        break;
    }
  }

  return !stream.hasError();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Removes entry associated with given index. */
void ResourceLibraryDataModel::removeItem(const QModelIndex& index)
{
  // get item form model
  ResourceItem* item = getItem(index);
  Q_ASSERT(item && item->parent());

	beginRemoveRows(index.parent(), item->row(), item->row());
  item->parent()->removeChild(item);
	endRemoveRows();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Inserts item after given index. */
QModelIndex ResourceLibraryDataModel::insertItem(const QModelIndex& index, ResourceItem* item)
{
  // get item form model
  ResourceItem* parentItem = getItem(index);

  // add to the end of the pool
  beginInsertRows(index.parent(), parentItem->childCount(), parentItem->childCount());
  parentItem->addChild(item);
  endInsertRows();

  return createIndex(parentItem->childCount() - 1, 0, item);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
