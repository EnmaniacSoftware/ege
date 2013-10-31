#ifndef RESOURCELIBRARY_RESOURCEITEMCONTAINER_H
#define RESOURCELIBRARY_RESOURCEITEMCONTAINER_H

/*! Specialization of resource item. This class serves as a container for other types of resource items, including other containers. */

#include "ResourceItem.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceItemContainer : public ResourceItem
{
  public:

    ResourceItemContainer();
   ~ResourceItemContainer();

  public:

    /*! Creates instance of resource item. This method is a registration method for factory. */
    static ResourceItem* Create();

  private:

    /*! @see ResourceItem::flags. */
    Qt::ItemFlags flags() const override;
    /*! @see ResourceItem::type. */
    QString type() const override;
    /*! @see ResourceItem::serialize. */
    bool serialize(QXmlStreamWriter& stream) const override;
    /*! @see ResourceItem::unserialize. */
    bool unserialize(QXmlStreamReader& stream) override;
    /*! @see ResourceItem::sizeHint. */
    QSize sizeHint() const override;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // RESOURCELIBRARY_RESOURCEITEMCONTAINER_H
