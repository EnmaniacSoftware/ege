#include "ResourceItem.h"
#include "ResourceLibraryDataModel.h"
#include <QImageReader>

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

    case ResourceLibraryDataModel::PathRole:

      return path();
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

    case ResourceLibraryDataModel::PathRole:

      m_path = value.toString();
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
/*! Sets path. */
void ResourceItem::setPath(const QString& path)
{
  m_path = path;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns the item flags for the given item. */
Qt::ItemFlags ResourceItem::flags() const
{
  return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! ISerializer override. Serializes into given buffer. */
QString ResourceItem::serialize() const
{
  QString data;

  return data;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! ISerializer override. Unserializes from given data buffer. */
bool ResourceItem::unserialize(const QString& data)
{
  return true;
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
