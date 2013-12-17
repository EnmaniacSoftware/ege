#include "ResourceItem.h"
#include "ResourceLibraryDataModel.h"
#include "MainWindow.h"
#include "ResourceItemFactory.h"
#include <QMessageBox>
#include <QDebug>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
// TAGE - duplicate in ResourceLibraryDataModel :/
static const QString KResourceItemTag = "ResourceItem";
static const QString KTypeAttribute   = "type";
static const QString KNameAttribute   = "name";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceItem::ResourceItem() : m_parent(NULL)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceItem::ResourceItem(const QString& name, ResourceItem* parent) : m_name(name),
                                                                        m_parent(parent)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceItem::~ResourceItem()
{
  qDeleteAll(m_children);
  m_children.clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
int ResourceItem::columnCount() const
{
  return 1;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
int ResourceItem::childCount() const
{
  return m_children.count();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
int ResourceItem::row() const
{
  if (NULL != m_parent)
  {
    return m_parent->m_children.indexOf(const_cast<ResourceItem*>(this));
  }

  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QVariant ResourceItem::data(int columnIndex, int role) const
{
  Q_UNUSED(columnIndex);

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
ResourceItem* ResourceItem::child(int index) const
{
  return ((0 <= index) && (index < childCount())) ? m_children[index] : NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ResourceItem::insertChildren(int position, int count, int columns)
{
  Q_UNUSED(columns);

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
void ResourceItem::setName(const QString& name)
{
  m_name = name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const QString& ResourceItem::name() const
{
  return m_name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Qt::ItemFlags ResourceItem::flags() const
{
  return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ResourceItem::beginSerialize(QXmlStreamWriter& stream) const
{
  stream.writeStartElement(KResourceItemTag);
  
  stream.writeAttribute(KTypeAttribute, type());
  stream.writeAttribute(KNameAttribute, name());

  return ! stream.hasError();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ResourceItem::endSerialize(QXmlStreamWriter& stream) const
{
  // serialize children
  foreach (const ResourceItem* item, m_children)
  {
    if ( ! item->serialize(stream))
    {
      // error!
      return false;
    }
  }

  // finalize
  stream.writeEndElement();

  return ! stream.hasError();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceItem::setParent(ResourceItem* parent)
{
  m_parent = parent;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceItem* ResourceItem::parent() const
{
  return m_parent;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QString ResourceItem::type() const
{
  return "generic";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceItem::setChild(int index, ResourceItem* item)
{
  Q_ASSERT(index < m_children.size());
  m_children[index] = item;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QSize ResourceItem::sizeHint() const
{
  return QSize();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceItem::removeChild(ResourceItem* item)
{
  int index = m_children.indexOf(item);
  if (0 <= index)
  {
    m_children.removeAt(index);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
int ResourceItem::childIndex(ResourceItem* item) const
{
  return m_children.indexOf(item);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceItem::addChild(ResourceItem* item)
{
  // set child's parent
  item->setParent(this);

  // add to pool
  m_children.append(item);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceItem::removeChildren()
{
  qDeleteAll(m_children);
  m_children.clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ResourceItem::serialize(QXmlStreamWriter& stream) const
{
  Q_UNUSED(stream);

  // This method should never be called
  Q_ASSERT(false);

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ResourceItem::unserialize(QXmlStreamReader& stream)
{
  Q_UNUSED(stream);

  // This method should never be called
  Q_ASSERT(false);

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
