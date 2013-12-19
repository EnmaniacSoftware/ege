#ifndef RESOURCELIBRARY_RESOURCEITEMIMAGE_H
#define RESOURCELIBRARY_RESOURCEITEMIMAGE_H

#include <QImage>
#include "ResourceItem.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class QMenu;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceItemImage : public ResourceItem
{
  public:

    ResourceItemImage();
    ResourceItemImage(const QString& name, ResourceItem* parent);
   ~ResourceItemImage();

  public:

    /*! Creates instance of resource item. This method is a registration method for factory. */
    static ResourceItem* Create(const QString& name, ResourceItem* parent);
    /*! Returns item type name. */
    static QString TypeName();
    /*! Hooks into Resource Library Window context menu.
     *  @param  menu          Menu to hook into.
     *  @param  selectedType  Name of the type of resource item for which context menu is being built.
     */
    static void ResourceLibraryWindowHook(QMenu& menu, const QString& selectedType);

  public:

    /*! Returns thumbnail image.
     *  @note Generates thumbnail image if required.
     */
    const QImage& thumbnailImage() const;
    /*! Sets path. */
    void setPath(const QString& path);
    /*! Returns path to asset. */
    const QString& path() const;

  private:

    /*! @see ResourceItem::type. */
    QString type() const override;
    /*! @see ResourceItem::sizeHint. */
    QSize sizeHint() const override;
    /*! @see ResourceItem::serialize. */
    bool serialize(QXmlStreamWriter& stream) const override;
    /*! @see ResourceItem::unserialize. */
    bool unserialize(QXmlStreamReader& stream) override;
    /*! @see ResourceItem::data. */
    QVariant data(int columnIndex, int role) const override;
    /*! @see ResourceItem::setData. */
    bool setData(const QVariant &value, int role) override;

  private:

    /*! Thumbnail image. Only valid for specific types. */
    mutable QImage m_thumbnail;
    /*! Path to asset. */
    QString m_path;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // RESOURCELIBRARY_RESOURCEITEMIMAGE_H
