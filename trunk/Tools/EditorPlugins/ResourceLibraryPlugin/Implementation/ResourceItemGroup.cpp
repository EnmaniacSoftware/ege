#include "ResourceItemGroup.h"
#include "ResourceLibraryWindowResourceInserter.h"
#include <ObjectPool.h>
#include <QDebug>
#include <QMenu>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
using NPropertyObject::PropertyDefinition;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceItemGroup::ResourceItemGroup(const QString& name, const QString& configurationName, ResourceItem* parent)
  : ResourceItem(name, configurationName, parent)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceItemGroup::~ResourceItemGroup()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceItem* ResourceItemGroup::Create(const QString& name, const QString& configurationName, ResourceItem* parent)
{
  return new ResourceItemGroup(name, configurationName, parent);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QString ResourceItemGroup::TypeName()
{
  return "group";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceItemGroup::ResourceLibraryWindowHook(QMenu& menu, const QString& selectedType)
{
  // only allow for top level items
  if (selectedType.isEmpty())
  {
    ResourceLibraryWindowResourceInserter* helper = ObjectPool::Instance()->getObject<ResourceLibraryWindowResourceInserter>();
    Q_ASSERT(NULL != helper);

    menu.addAction(tr("Add group"), helper, SLOT(onAddGroup()));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Qt::ItemFlags ResourceItemGroup::flags() const
{
  return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QString ResourceItemGroup::typeName() const
{
  return ResourceItemGroup::TypeName();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ResourceItemGroup::serialize(QXmlStreamWriter& stream) const
{
  bool result = false;

  // begin serialization
  if (beginSerialize(stream))
  {
    // end serialization
    result = endSerialize(stream);
  }

  return result && ! stream.hasError();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ResourceItemGroup::unserialize(QXmlStreamReader& stream)
{
  // nothing to do
  return ! stream.hasError();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QSize ResourceItemGroup::sizeHint() const
{
  return QSize(200, 20);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QList<PropertyDefinition> ResourceItemGroup::propertiesDefinition() const
{
  // call base class first
  QList<PropertyDefinition> list = ResourceItem::propertiesDefinition();

  return list;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
