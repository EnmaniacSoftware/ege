#ifndef RESOURCELIBRARY_RESOURCEITEMCONTAINER_H
#define RESOURCELIBRARY_RESOURCEITEMCONTAINER_H

/*! Specialization of resource item.
 *  This class serves as a container (group) for other types of resource items.
 */

#include "ResourceItem.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class QMenu;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceItemGroup : public ResourceItem
{
  public:

   ~ResourceItemGroup();

  public:

    /*! Creates instance of resource item. This method is a registration method for factory. */
    static ResourceItem* Create(const QString& name, const QString& configurationName, ResourceItem* parent);
    /*! Returns item type name. */
    static QString TypeName();
    /*! Hooks into Resource Library Window context menu.
     *  @param  menu          Menu to hook into.
     *  @param  selectedType  Name of the type of resource item for which context menu is being built.
     */
    static void ResourceLibraryWindowHook(QMenu& menu, const QString& selectedType);

  private:

    ResourceItemGroup(const QString& name, const QString& configurationName, ResourceItem* parent);

    /*! @see ResourceItem::flags. */
    Qt::ItemFlags flags() const override;
    /*! @see ResourceItem::typeName. */
    QString typeName() const override;
    /*! @see ResourceItem::serialize. */
    bool serialize(QXmlStreamWriter& stream) const override;
    /*! @see ResourceItem::unserialize. */
    bool unserialize(QXmlStreamReader& stream) override;
    /*! @see ResourceItem::sizeHint. */
    QSize sizeHint() const override;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // RESOURCELIBRARY_RESOURCEITEMCONTAINER_H
