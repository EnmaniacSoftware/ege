#ifndef RESOURCELIBRARY_RESOURCEITEMTEXTURE_H
#define RESOURCELIBRARY_RESOURCEITEMTEXTURE_H

/*! Specialization of resource item.
 *  Class representing any kind of texture within Resource Library.
 */

#include <QImage>
#include "ResourceItem.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Available texture types. */
enum TextureType
{
  EInvalidTexture,
  ETexture2D
};

/*! Available types for texture filters. */
enum TextureFilterTypes
{
  ETextureFilterNearest,
  ETextureFilterBilinear
};

/*! Available types for mip mapping (minification) filtering. */
enum TextureMipMappingFilterTypes
{
  EMipMappingFilterNone,
  EMipMappingFilterNearest,
  EMipMappingFilterTrilinear
};

/*! Available texture addressing modes. */
enum TextureAddressModeTypes
{
  EAddressModeClamp,
  EAddressModeRepeat
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class QMenu;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceItemTexture : public ResourceItem
{
  Q_OBJECT

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

  private slots:

    /*! Slot called when internal (lazy) data should be invalidated. */
    void onInvalidate();

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
    /*! Adds minification filters definitions to given group.
     *  @param  group Definition group to which minification filters are to be added.
     */
    void addMinificationFilterDefinitions(NPropertyObject::PropertyDefinition& group) const;
    /*! Adds magnification filters definitions to given group.
     *  @param  group Definition group to which magnification filters are to be added.
     */
    void addMagnificationFilterDefinitions(NPropertyObject::PropertyDefinition& group) const;
    /*! Adds texture addressing modes definitions to given group.
     *  @param  group Definition group to which addressing modes are to be added.
     */
    void addAddressingModeDefinitions(NPropertyObject::PropertyDefinition& group) const;
    /*! @see ResourceItem::update. */
    void update(const QString& name, const QVariant& value) override;
    /*! Returns texture type as string. */
    QString textureTypeName() const;
    /*! Returns texture type from string. */
    TextureType textureTypeFromString(const QString& typeName) const;
    /*! Returns image size (in pixels). */
    QSize size() const;
    /*! Returns image format. */
    QImage::Format imageFormat() const;
    /*! Returns image format in string format. */
    QString imageFormatAsText() const;

    /*! Sets minification filter. */
    void setMinificationFilter(TextureFilterTypes type);
    /*! Sets magnification filter. */
    void setMagnificationFilter(TextureFilterTypes type);
    /*! Sets mip mapping filter. */
    void setMipMappingFilter(TextureMipMappingFilterTypes type);
    /*! Sets texture addressing mode for S coordinate. */
    void setAddressModeS(TextureAddressModeTypes type);
    /*! Sets texture addressing mode for T coordinate. */
    void setAddressModeT(TextureAddressModeTypes type);

  private:

    /*! Thumbnail image. Only valid for specific types. */
    mutable QImage m_thumbnail;
    /*! Path to asset. */
    QString m_path;
    /*! Texture type. */
    TextureType m_type;
    /*! Size of the image (in pixels). */
    mutable QSize m_size;
    /*! Image format. */
    mutable QImage::Format m_imageFormat;
    /*! Magnification filter. */
    TextureFilterTypes m_magnificationFilter;
    /*! Minification filter. */
    TextureFilterTypes m_minificationFilter;
    /*! Mip Mapping filter. */
    TextureMipMappingFilterTypes m_mipMappingFilter;
    /*! Addressing mode for S coordinate. */
    TextureAddressModeTypes m_addressingModeS;
    /*! Addressing mode for T coordinate. */
    TextureAddressModeTypes m_addressingModeT;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // RESOURCELIBRARY_RESOURCEITEMTEXTURE_H
