#include "ResourceItemContainer.h"
#include <QDebug>

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
  stream.writeStartElement("resource-item");
  
  stream.writeAttribute("type", type());
  stream.writeAttribute("name", name());

  // serialize children
  foreach (const ResourceItem* item, m_children)
  {
    if ( ! item->serialize(stream))
    {
      // error!
      return false;
    }
  }

  stream.writeEndElement();
  return !stream.hasError();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ResourceItemContainer::unserialize(QXmlStreamReader& stream)
{
  return ResourceItem::unserialize(stream);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QSize ResourceItemContainer::sizeHint() const
{
  return QSize(200, 20);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
