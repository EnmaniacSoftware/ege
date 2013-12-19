#include "ResourceItemContainer.h"
#include "ResourceLibraryWindowGroupAdder.h"
#include <ObjectPool.h>
#include <QDebug>
#include <QMenu>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceItemContainer::ResourceItemContainer() : ResourceItem()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceItemContainer::ResourceItemContainer(const QString& name, ResourceItem* parent) : ResourceItem(name, parent)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceItemContainer::~ResourceItemContainer()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceItem* ResourceItemContainer::Create(const QString& name, ResourceItem* parent)
{
  return new ResourceItemContainer(name, parent);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QString ResourceItemContainer::TypeName()
{
  return "container";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceItemContainer::ResourceLibraryWindowHook(QMenu& menu, const QString& selectedType)
{
  if (selectedType.isEmpty() || (selectedType == TypeName()))
  {
    ResourceLibraryWindowGroupAdder* adder = ObjectPool::Instance()->getObject<ResourceLibraryWindowGroupAdder>();
    Q_ASSERT(NULL != adder);

    menu.addAction(tr("Add group"), adder, SLOT(onAdd()));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Qt::ItemFlags ResourceItemContainer::flags() const
{
  return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QString ResourceItemContainer::type() const
{
  return ResourceItemContainer::TypeName();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ResourceItemContainer::serialize(QXmlStreamWriter& stream) const
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
bool ResourceItemContainer::unserialize(QXmlStreamReader& stream)
{
  Q_UNUSED(stream);

  // nothing to do
  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QSize ResourceItemContainer::sizeHint() const
{
  return QSize(200, 20);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
