#include "ResourceLibraryDataModel.h"
#include "ResourceLibraryItem.h"
#include <QFile>
#include <QXmlStreamReader>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceLibraryDataModel::ResourceLibraryDataModel(QObject* parent) : QAbstractItemModel(parent),
                                                                      m_root(NULL)
{
  createDefault();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceLibraryDataModel::~ResourceLibraryDataModel()
{
  clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! QAbstractItemModel override. Returns the parent of the model item with the given index. If the item has no parent, an invalid QModelIndex is returned. */
QModelIndex ResourceLibraryDataModel::parent(const QModelIndex& index) const
{
  if (!index.isValid())
  {
    return QModelIndex();
  }

  ResourceLibraryItem* childItem = static_cast<ResourceLibraryItem*>(index.internalPointer());
  ResourceLibraryItem* parentItem = childItem->parent();

  if (parentItem == m_root)
  {
    return QModelIndex();
  }

  return createIndex(parentItem->row(), 0, parentItem);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! QAbstractItemModel override. Returns the index of the item in the model specified by the given row, column and parent index. */
QModelIndex ResourceLibraryDataModel::index(int row, int column, const QModelIndex& parent) const
{
  ResourceLibraryItem* parentItem;
  ResourceLibraryItem* item;

  // check if parent invalid
  if (!parent.isValid())
  {
    parentItem = m_root;
  }
  else
  {
    parentItem = static_cast<ResourceLibraryItem*>(parent.internalPointer());
  }

  // get proper child
  item = parentItem->child(row);
  if (NULL == item)
  {
    return QModelIndex();
  }

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

  if (Qt::DisplayRole != role)
  {
    // done
    return QVariant();
  }

  ResourceLibraryItem* modelItem = static_cast<ResourceLibraryItem*>(index.internalPointer());
  return modelItem->data(index.column());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! QAbstractItemModel override. Returns the number of columns for the children of the given parent. */
int ResourceLibraryDataModel::columnCount(const QModelIndex& parent) const
{
  if (parent.isValid())
  {
      return static_cast<ResourceLibraryItem*>(parent.internalPointer())->columnCount();
  }
  
  return m_root->columnCount();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! QAbstractItemModel override. Returns the number of rows under the given parent. */
int ResourceLibraryDataModel::rowCount(const QModelIndex& parent) const
{
  ResourceLibraryItem* parentItem;
  if (parent.column() > 0)
      return 0;

  if (!parent.isValid())
  {
    parentItem = m_root;
  }
  else
  {
    parentItem = static_cast<ResourceLibraryItem*>(parent.internalPointer());
  }

  return parentItem->childCount();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Loads data from file at given path. */
bool ResourceLibraryDataModel::load(const QString& path)
{
  QFile file(path);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    // error!
    return false;
  }

  QXmlStreamReader xmlReader(&file);
  while (!xmlReader.atEnd() && !xmlReader.hasError())
  {
    // read next element
    QXmlStreamReader::TokenType token = xmlReader.readNext();

    // check if document start
    if (QXmlStreamReader::StartDocument == token)
    {
      // skip
      continue;
    }

    // check element start
    if (QXmlStreamReader::StartElement == token) 
    {
      // process
      if ("resource-item" == xmlReader.name())
      {
        QXmlStreamAttributes attributes = xmlReader.attributes();
      }

      //if(xmlReader->name() == "id") {
      //    qDebug() << xmlReader->readElementText();
      //}
    }
  }

  return !xmlReader.hasError();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Saves data to file at given path. */
bool ResourceLibraryDataModel::save(const QString& path)
{
  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Clears data. */
void ResourceLibraryDataModel::clear()
{
  if (m_root)
  {
    delete m_root;
    m_root = NULL;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Adds */
void ResourceLibraryDataModel::add(ResourceLibraryItem* item)
{
  m_root->add(item);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates default. */
void ResourceLibraryDataModel::createDefault()
{
  m_root = new ResourceLibraryItem(tr("Resources"), QString(), ResourceLibraryItem::TYPE_CONTAINER);
  m_root->add(new ResourceLibraryItem(tr("Graphics"), QString(), ResourceLibraryItem::TYPE_CONTAINER));
  m_root->add(new ResourceLibraryItem(tr("Sounds"), QString(), ResourceLibraryItem::TYPE_CONTAINER));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
