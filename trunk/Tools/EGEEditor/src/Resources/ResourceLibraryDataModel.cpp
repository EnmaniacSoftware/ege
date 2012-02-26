#include "ResourceLibraryDataModel.h"
#include "Resources/ResourceItem.h"
#include "Resources/ResourceItemFactory.h"
#include "MainWindow.h"
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

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
  //if (NULL == item)
  //{
  //  return QModelIndex();
  //}

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
/*! Loads data from given string. */
bool ResourceLibraryDataModel::load(const QString& data)
{
  QXmlStreamReader xmlReader(data);
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
/*! Saves data to string. */
QString ResourceLibraryDataModel::save() const
{
  QString output;

  QXmlStreamWriter stream(&output);
  stream.setAutoFormatting(true);
  stream.writeStartDocument();
  stream.writeStartElement("bookmark");
  stream.writeAttribute("href", "http://qt.nokia.com/");
  stream.writeTextElement("title", "Qt Home");
  stream.writeEndElement(); // bookmark

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
/*! QAbstractItemModel override. On models that support this, inserts count rows into the model before the given row. 
 *  Items in the new row will be children of the item represented by the parent model index.
 */
bool ResourceLibraryDataModel::insertRows(int position, int rows, const QModelIndex& parent)
{
  ResourceItem* parentItem = getItem(parent);

  bool success;

  // insert rows
  beginInsertRows(parent, position, position + rows - 1);
  success = parentItem->insertChildren(position, rows, m_root->columnCount());
  endInsertRows();

  return success;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ResourceLibraryDataModel::removeRows(int position, int rows, const QModelIndex& parent)
{
    ResourceItem* parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    //success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
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

  // by default incoming model index is what will be changed
  QModelIndex modelIndex = index;

  // check if item should be re-allocated
  if ((ResourceLibraryDataModel::TypeRole == role) && (value.toString() != item->type()))
  {
    // get parent item
    ResourceItem* parentItem = getItem(parent(index));

    // create object
    ResourceItem* newItem = resourceItemFactory()->createItem(value.toString(), QString(), item->parent());
    if (NULL == newItem)
    {
      // error!
      return false;
    }

    // delete old item
    delete item;

    // insert newly created item into its placeholder
    parentItem->setChild(index.column(), newItem);

    // update model index changed
    modelIndex = createIndex(index.row(), index.column(), newItem);
  }

  // process according to role
  bool success = item->setData(value, role);

  // emit
  if (success)
  {
    emit dataChanged(modelIndex, modelIndex);
  }

  return success;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Returns pointer to resource item factory object. */
ResourceItemFactory* ResourceLibraryDataModel::resourceItemFactory() const
{
  return reinterpret_cast<MainWindow*>(QObject::parent()->parent())->resourceItemFactory();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
