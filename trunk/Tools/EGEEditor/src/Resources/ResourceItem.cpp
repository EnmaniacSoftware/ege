#include "ResourceItem.h"
#include "ResourceLibraryDataModel.h"
#include "MainWindow.h"
#include "ResourceItemFactory.h"
#include <QMessageBox>
#include <QDebug>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceItem::ResourceItem() : m_parent(NULL)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceItem::~ResourceItem()
{
  qDeleteAll(m_children);
  m_children.clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns number of columns. */
int ResourceItem::columnCount() const
{
  return 1;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns children count. */
int ResourceItem::childCount() const
{
  return m_children.count();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns row index at which current item is placed withing parent. */
int ResourceItem::row() const
{
  if (NULL != m_parent)
  {
    return m_parent->m_children.indexOf(const_cast<ResourceItem*>(this));
  }

  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns data for a given column and role. 
 *  @param columnIndex Column index for which data is to be retrieved.
 *  @param role        Role for which data is to be retrieved.
 *  @return Returns data associated with a given role at given column. If no valid data is present returns empty QVariant.
 */
QVariant ResourceItem::data(int columnIndex, int role) const
{
  // process according to role
  switch (role)
  {
    case Qt::DisplayRole:
    case Qt::EditRole:

      return name();

    case ResourceLibraryDataModel::TypeRole:

      return type();
  }

  return QVariant();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets the role data. 
 *  @param value  Value to be set.
 *  @param role   Role for which data is set.
 *  @return Returns TRUE if data has been changed. Otherwise FALSE.
 */
bool ResourceItem::setData(const QVariant &value, int role)
{
  // process according to role
  switch (role)
  {
    case Qt::DisplayRole:
    case Qt::EditRole:

      m_name = value.toString();
      return true;

    case ResourceLibraryDataModel::TypeRole:

      // just return success
      return true;
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns child at given index. */
ResourceItem* ResourceItem::child(int index) const
{
  return ((0 <= index) && (index < childCount())) ? m_children[index] : NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Inserts children. */
bool ResourceItem::insertChildren(int position, int count, int columns)
{
  // check if insertion within proper range
  if ((0 > position) || (position > m_children.size()))
  {
    // error!
    return false;
  }

  for (int row = 0; row < count; ++row)
  {
    ResourceItem* object = new ResourceItem();
    if (NULL == object)
    {
      // error!
      return false;
    }

    object->setParent(this);

    // insert default object
    // NOTE: object shall be reallocated once the type is known
    m_children.insert(position, object);
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets name. */
void ResourceItem::setName(const QString& name)
{
  m_name = name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns the item flags for the given item. */
Qt::ItemFlags ResourceItem::flags() const
{
  return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! ISerializer override. Serializes into given stream. */
bool ResourceItem::serialize(QXmlStreamWriter& stream) const
{
  stream.writeStartElement("resource-item");
  
  stream.writeAttribute("type", type());
  stream.writeAttribute("name", name());

  // serialize children
  foreach (const ResourceItem* item, m_children)
  {
    if (!item->serialize(stream))
    {
      // error!
      return false;
    }
  }

  stream.writeEndElement();

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! ISerializer override. Unserializes from given data stream. */
bool ResourceItem::unserialize(QXmlStreamReader& stream)
{
  QString name = stream.attributes().value("name").toString();
  QString type = stream.attributes().value("type").toString();

  //Q_ASSERT((m_name == stream.attributes().value("name")) && (type() == stream.attributes().value("type")));

  // process children
  bool done = false;
  while (!stream.atEnd() && !done)
  {
    QXmlStreamReader::TokenType token = stream.readNext();
    switch (token)
    {
      case QXmlStreamReader::StartElement:

        // check if resource item element
        if ("resource-item" == stream.name())
        {
          // create proper item
          ResourceItem* item = app->resourceItemFactory()->createItem(stream.attributes().value("type").toString(), 
                                                                      stream.attributes().value("name").toString(), this);

          // check if processing an item already
          if (NULL != item)
          {
            // let item unserialize
            if (!item->unserialize(stream))
            {
              // error!
              delete item;
              return false;
            }
 
            // add to pool
            m_children.push_back(item);
          }
          else
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
/*! Sets parent. */
void ResourceItem::setParent(ResourceItem* parent)
{
  m_parent = parent;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns type name. */
QString ResourceItem::type() const
{
  return "generic";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets child item. */
void ResourceItem::setChild(int index, ResourceItem* item)
{
  Q_ASSERT(index < m_children.size());
  m_children[index] = item;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns size hint. */
QSize ResourceItem::sizeHint() const
{
  return QSize();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Removes given child. */
void ResourceItem::removeChild(ResourceItem* item)
{
  int index = m_children.indexOf(item);
  if (0 <= index)
  {
    m_children.removeAt(index);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns index of a given child. */
int ResourceItem::childIndex(ResourceItem* item) const
{
  return m_children.indexOf(item);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Adds child to the end of pool. */
void ResourceItem::addChild(ResourceItem* item)
{
  // set child's parent
  item->setParent(this);

  // add to pool
  m_children.append(item);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Removes all children. */
void ResourceItem::removeChildren()
{
  qDeleteAll(m_children);
  m_children.clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
