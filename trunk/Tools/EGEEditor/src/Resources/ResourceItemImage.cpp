#include "ResourceItemImage.h"
#include "ResourceLibraryDataModel.h"
#include <QImageReader>

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
