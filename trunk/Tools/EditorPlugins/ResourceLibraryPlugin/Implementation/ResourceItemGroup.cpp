#include "ResourceItemGroup.h"
#include "ResourceLibraryWindowGroupAdder.h"
#include <ObjectPool.h>
#include <QDebug>
#include <QMenu>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceItemGroup::ResourceItemGroup() : ResourceItem()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceItemGroup::ResourceItemGroup(const QString& name, ResourceItem* parent) : ResourceItem(name, parent)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceItemGroup::~ResourceItemGroup()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceItem* ResourceItemGroup::Create(const QString& name, ResourceItem* parent)
{
  return new ResourceItemGroup(name, parent);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QString ResourceItemGroup::TypeName()
{
  return "container";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceItemGroup::ResourceLibraryWindowHook(QMenu& menu, const QString& selectedType)
{
  // only allow for top level items
  if (selectedType.isEmpty())// || (selectedType == TypeName()))
  {
    ResourceLibraryWindowGroupAdder* adder = ObjectPool::Instance()->getObject<ResourceLibraryWindowGroupAdder>();
    Q_ASSERT(NULL != adder);

    menu.addAction(tr("Add group"), adder, SLOT(onAdd()));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Qt::ItemFlags ResourceItemGroup::flags() const
{
  return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QString ResourceItemGroup::type() const
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
  Q_UNUSED(stream);

  // nothing to do
  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QSize ResourceItemGroup::sizeHint() const
{
  return QSize(200, 20);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
