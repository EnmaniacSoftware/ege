#include "ResourceLibraryItem.h"
#include "ResourceLibraryDataModel.h"
#include <QImageReader>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceLibraryItem::ResourceLibraryItem(ResourceLibraryItem* parent) : m_parent(parent),
                                                                        m_type(TYPE_NONE)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceLibraryItem::~ResourceLibraryItem()
{
  qDeleteAll(m_children);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns number of columns. */
int ResourceLibraryItem::columnCount() const
{
  return 1;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns children count. */
int ResourceLibraryItem::childCount() const
{
  return m_children.count();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns row index at which current item is placed withing parent. */
int ResourceLibraryItem::row() const
{
  if (NULL != m_parent)
  {
    return m_parent->m_children.indexOf(const_cast<ResourceLibraryItem*>(this));
  }

  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns data for a given column and role. 
 *  @param columnIndex Column index for which data is to be retrieved.
 *  @param role        Role for which data is to be retrieved.
 *  @return Returns data associated with a given role at given column. If no valid data is present returns empty QVariant.
 */
QVariant ResourceLibraryItem::data(int columnIndex, int role) const
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
bool ResourceLibraryItem::setData(const QVariant &value, int role)
{
  // process according to role
  switch (role)
  {
    case Qt::DisplayRole:
    case Qt::EditRole:

      m_name = value.toString();
      return true;

    case ResourceLibraryDataModel::TypeRole:

      m_type = static_cast<ResourceLibraryItem::Type>(value.toInt());
      return true;

    case ResourceLibraryDataModel::PathRole:

      m_path = value.toString();
      return true;
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Adds child. */
//void ResourceLibraryItem::add(ResourceLibraryItem* child)
//{
//  // set parent
//  child->m_parent = this;
//
//  // append
//  m_children.append(child);
//}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns child at given index. */
ResourceLibraryItem* ResourceLibraryItem::child(int index) const
{
  return ((0 <= index) && (index < childCount())) ? m_children[index] : NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Inserts children. */
bool ResourceLibraryItem::insertChildren(int position, int count, int columns)
{
  // check if insertion within proper range
  if ((0 > position) || (position > m_children.size()))
  {
    // error!
    return false;
  }

  for (int row = 0; row < count; ++row)
  {
    ResourceLibraryItem* item = new ResourceLibraryItem(this);
    m_children.insert(position, item);
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets name. */
void ResourceLibraryItem::setName(const QString& name)
{
  m_name = name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets type. */
void ResourceLibraryItem::setType(Type type)
{
  m_type = type;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets path. */
void ResourceLibraryItem::setPath(const QString& path)
{
  m_path = path;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns thumbnail image. 
 *  @note Generates thumbnail image if required. 
 */
const QImage& ResourceLibraryItem::thumbnailImage() const
{
  // check if thumbnail is to be generated
  if ((TYPE_IMAGE == type()) && m_thumbnail.isNull() && !m_path.isEmpty() && !m_name.isEmpty())
  {
    // generate
    QImageReader imageReader(m_path + "/" + m_name);

    imageReader.setScaledSize(QSize(32, 32));
    imageReader.read(&m_thumbnail);
  }

  return m_thumbnail;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
