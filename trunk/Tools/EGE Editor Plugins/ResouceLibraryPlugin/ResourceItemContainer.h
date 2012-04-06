#ifndef RESOURCE_ITEM_CONTAINER_H
#define RESOURCE_ITEM_CONTAINER_H

#include "Resources/ResourceItem.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Container resource item. */
class ResourceItemContainer : public ResourceItem
{
  public:

    ResourceItemContainer();
   ~ResourceItemContainer();

  public:

    /* Creates instance of resource item. This method is a registration method for factory. */
    static ResourceItem* Create();

  private:

    /* ResourceItem override. Returns the item flags for the given item. */
    Qt::ItemFlags flags() const override;
    /*! ResourceItem override. Returns type name. */
    QString type() const override;
    /* ResourceItem override. Serializes into given stream. */
    bool serialize(QXmlStreamWriter& stream) const override;
    /* ResourceItem override. Unserializes from given data stream. */
    bool unserialize(QXmlStreamReader& stream) override;
    /* ResourceItem override. Returns size hint. */
    QSize sizeHint() const override;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // RESOURCE_ITEM_CONTAINER_H