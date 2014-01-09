#include "ResourceItem.h"
#include "ResourceLibraryDataModel.h"
#include "MainWindow.h"
#include "ResourceItemFactory.h"
#include <QMessageBox>
#include <QDebug>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
// TAGE - duplicate in ResourceLibraryDataModel :/
static const QString KResourceItemTag         = "ResourceItem";
static const QString KTypeAttribute           = "type";
static const QString KNameAttribute           = "name";
static const QString KConfigurationAttribute  = "configuration";

static const QString KGeneralGroupName = QObject::tr("Name");
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceItem::ResourceItem(const QString& name, const QString& configurationName, ResourceItem* parent) : m_name(name),
                                                                                                          m_parent(parent),
                                                                                                          m_configurationName(configurationName)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceItem::~ResourceItem()
{
  qDeleteAll(m_children);
  m_children.clear();
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

  QVariant value;

  // process according to role
  switch (role)
  {
    case Qt::DisplayRole:
    case Qt::EditRole:

      value = name();
      break;

    case ResourceLibraryDataModel::TypeRole:

      value = typeName();
      break;

    case ResourceLibraryDataModel::ConfigRole:

      value = m_configurationName;
      break;
  }

  return value;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ResourceItem::setData(const QVariant& value, int role)
{
  // process according to role
  switch (role)
  {
    case Qt::DisplayRole:
    case Qt::EditRole:

      setName(value.toString());
      return true;

    case ResourceLibraryDataModel::TypeRole:

      // just return success
      return true;

    case ResourceLibraryDataModel::ConfigRole:

      m_configurationName = value.toString();
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
    // TAGE - Check it...
    ResourceItem* object = NULL;// = new ResourceItem();
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
  if (m_name != name)
  {
    m_name = name;

    // emit
    emit changed(this);
  }
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
  
  stream.writeAttribute(KTypeAttribute, typeName());
  stream.writeAttribute(KNameAttribute, name());
  stream.writeAttribute(KConfigurationAttribute, m_configurationName);

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
QList<NPropertyObject::PropertyDefinition> ResourceItem::propertiesDefinition() const
{
  QList<NPropertyObject::PropertyDefinition> list;

  // create general group
  NPropertyObject::PropertyDefinition generalGroup(tr("General"), NPropertyObject::EGroup);

  // add name
  NPropertyObject::PropertyDefinition generalName(KGeneralGroupName, NPropertyObject::EString, name());
  generalGroup.addChildProperty(generalName);

  // add groups to list
  list.push_back(generalGroup);

  return list;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceItem::update(const QString& name, const QVariant& value)
{
  if (KGeneralGroupName == name)
  {
    Q_ASSERT(value.canConvert(QVariant::String));

    setName(value.toString());
  }
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
QString ResourceItem::typeName() const
{
  return "generic";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const QString&ResourceItem::configurationName() const
{
  return m_configurationName;
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
