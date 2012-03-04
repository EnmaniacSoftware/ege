#include "ResourceItemContainer.h"
#include "ResourceLibraryDataModel.h"
#include <QImageReader>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceItemContainer::ResourceItemContainer() : ResourceItem()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceItemContainer::~ResourceItemContainer()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance of resource item. This method is a registration method for factory. */
ResourceItem* ResourceItemContainer::Create()
{
  return new ResourceItemContainer();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! ResourceItem override. Returns the item flags for the given item. */
Qt::ItemFlags ResourceItemContainer::flags() const
{
  return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! ResourceItem override. Returns type name. */
QString ResourceItemContainer::type() const
{
  return "container";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! ResourceItem override. Serializes into given stream. */
bool ResourceItemContainer::serialize(QXmlStreamWriter& stream) const
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
  return !stream.hasError();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! ResourceItem override. Unserializes from given data stream. */
bool ResourceItemContainer::unserialize(const QXmlStreamReader& stream)
{
  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! ResourceItem override. Returns size hint. */
QSize ResourceItemContainer::sizeHint() const
{
  return QSize(200, 20);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
