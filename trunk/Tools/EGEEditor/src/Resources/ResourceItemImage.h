#ifndef RESOURCE_ITEM_IMAGE_H
#define RESOURCE_ITEM_IMAGE_H

#include <QImage>
#include "ResourceItem.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceItemImage : public ResourceItem
{
  public:

    ResourceItemImage();
   ~ResourceItemImage();

  public:

    /* Creates instance of resource item. This method is a registration method for factory. */
    static ResourceItem* Create();

  public:

    /* Returns thumbnail image. 
     * @note Generates thumbnail image if required. 
     */
    const QImage& thumbnailImage() const;

  private:

    /*! ResourceItem override. Returns type name. */
    QString type() const override;

  private:

    /*! Thumbnail image. Only valid for specific types. */
    mutable QImage m_thumbnail;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // RESOURCE_ITEM_IMAGE_H