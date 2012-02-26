#ifndef RESOURCE_ITEM_CONTAINER_H
#define RESOURCE_ITEM_CONTAINER_H

#include "Resources/ResourceItem.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
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
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // RESOURCE_ITEM_CONTAINER_H