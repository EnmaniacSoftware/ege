#ifndef RESOURCELIBRARY_RESOURCEITEMTEXTURE_H
#define RESOURCELIBRARY_RESOURCEITEMTEXTURE_H

/*! Specialization of resource item.
 *  Class representing any kind of texture within Resource Library.
 */

#include <QImage>
#include "ResourceItem.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
enum TextureType
{
  EInvalidTexture,
  ETexture2D
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class QMenu;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceItemTexture : public ResourceItem
{
  public:

   ~ResourceItemTexture();

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

  public:

    /*! Returns thumbnail image.
     *  @note Generates thumbnail image if required.
     */
    const QImage& thumbnailImage() const;
    /*! Sets path. */
    void setPath(const QString& path);
    /*! Returns path to asset. */
    const QString& path() const;
    /*! Sets texture type. */
    void setType(TextureType type);

  private:

    ResourceItemTexture(const QString& name, const QString& configurationName, ResourceItem* parent);

    /*! @see ResourceItem::typeName. */
    QString typeName() const override;
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
    /*! @see ResourceItem::propertiesDefinition. */
    QList<NPropertyObject::PropertyDefinition> propertiesDefinition() const override;
    /*! @see ResourceItem::update. */
    void update(const QString& name, const QVariant& value) override;
    /*! Returns texture type as string. */
    QString textureTypeName() const;
    /*! Returns texture type from string. */
    TextureType textureTypeFromString(const QString& typeName) const;

  private:

    /*! Thumbnail image. Only valid for specific types. */
    mutable QImage m_thumbnail;
    /*! Path to asset. */
    QString m_path;
    /*! Texture type. */
    TextureType m_type;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // RESOURCELIBRARY_RESOURCEITEMTEXTURE_H
