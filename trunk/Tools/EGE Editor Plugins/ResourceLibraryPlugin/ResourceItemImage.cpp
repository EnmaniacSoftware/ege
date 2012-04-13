#include "ResourceItemImage.h"
#include "ResourceLibraryDataModel.h"
#include "ResourceItemFactory.h"
#include "MainWindow.h"
#include <QImageReader>
#include <QDebug>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceItemImage::ResourceItemImage() : ResourceItem()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceItemImage::~ResourceItemImage()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance of resource item. This method is a registration method for factory. */
ResourceItem* ResourceItemImage::Create()
{
  return new ResourceItemImage();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns thumbnail image. 
 *  @note Generates thumbnail image if required. 
 */
const QImage& ResourceItemImage::thumbnailImage() const
{
  // check if thumbnail is to be generated
  if (m_thumbnail.isNull() && !m_path.isEmpty() && !m_name.isEmpty())
  {
    // generate
    QImageReader imageReader(m_path + "/" + m_name);

    imageReader.setScaledSize(QSize(32, 32));
    imageReader.read(&m_thumbnail);
  }

  return m_thumbnail;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! ResourceItem override. Returns type name. */
QString ResourceItemImage::type() const
{
  return "image";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! ResourceItem override. Returns size hint. */
QSize ResourceItemImage::sizeHint() const
{
  return QSize(200, 40);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! ResourceItem override. Serializes into given stream. */
bool ResourceItemImage::serialize(QXmlStreamWriter& stream) const
{
  stream.writeStartElement("resource-item");
  
  stream.writeAttribute("type", type());
  stream.writeAttribute("name", name());
  stream.writeAttribute("path", path());

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
bool ResourceItemImage::unserialize(QXmlStreamReader& stream)
{
  // retrieve data
  m_path = stream.attributes().value("path").toString();

  return !stream.hasError() && ResourceItem::unserialize(stream);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets path. */
void ResourceItemImage::setPath(const QString& path)
{
  m_path = path;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! ResourceItem override. Returns data for a given column and role. 
 *  @param columnIndex Column index for which data is to be retrieved.
 *  @param role        Role for which data is to be retrieved.
 *  @return Returns data associated with a given role at given column. If no valid data is present returns empty QVariant.
 */
QVariant ResourceItemImage::data(int columnIndex, int role) const
{
  // call base class first
  QVariant variant = ResourceItem::data(columnIndex, role);
  
  // process according to role
  switch (role)
  {
    case ResourceLibraryDataModel::PathRole:

      variant = path();
      break;
  }

  return variant;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! ResourceItem override. Sets the role data.
 *  @param value  Value to be set.
 *  @param role   Role for which data is set.
 *  @return Returns TRUE if data has been changed. Otherwise FALSE.
 */
bool ResourceItemImage::setData(const QVariant &value, int role)
{
  // process according to role
  switch (role)
  {
    case ResourceLibraryDataModel::PathRole:

      m_path = value.toString();
      return true;
  }

  // call base class
  return ResourceItem::setData(value, role);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
