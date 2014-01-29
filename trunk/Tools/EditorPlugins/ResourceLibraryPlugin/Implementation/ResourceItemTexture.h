#ifndef RESOURCELIBRARY_RESOURCEITEMTEXTURE_H
#define RESOURCELIBRARY_RESOURCEITEMTEXTURE_H

/*! Specialization of resource item.
 *  Class representing any kind of texture within Resource Library.
 */

#include <QImage>
#include "ResourceItem.h"
#include "TextureAddressingModes.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Available texture types. */
enum TextureType
{
  EInvalidTexture,
  ETexture2D
};

/*! Available types for texture filters. */
enum TextureFilterType
{
  ETextureFilterNearest,
  ETextureFilterBilinear
};

/*! Available types for mip mapping (minification) filtering. */
enum TextureMipMappingFilterType
{
  EMipMappingFilterNone,
  EMipMappingFilterNearest,
  EMipMappingFilterTrilinear
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class QMenu;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceItemTexture : public ResourceItem
{
  Q_OBJECT

  public:

   virtual ~ResourceItemTexture();

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
    virtual const QImage& thumbnailImage() const;
    /*! Sets full path including file name. */
    void setFullPath(const QString& path);
    /*! Returns full path to asset (including file name). */
    const QString& fullPath() const;
    /*! Sets texture type. */
    void setType(TextureType type);

  protected:

    ResourceItemTexture(const QString& name, const QString& configurationName, ResourceItem* parent);

    /*! @see ResourceItem::propertiesDefinition. */
    virtual QList<NPropertyObject::PropertyDefinition> propertiesDefinition() const override;
    /*! @see ResourceItem::update. */
    virtual void update(const QString& name, const QVariant& value) override;

    /*! Sets texture addressing mode for S coordinate. */
    void setAddressModeS(TextureAddressMode type);
    /*! Sets texture addressing mode for T coordinate. */
    void setAddressModeT(TextureAddressMode type);
    /*! Returns image format. */
    QImage::Format imageFormat() const;
    /*! Returns image size (in pixels). */
    virtual QSize size() const;

  protected:

    /*! Thumbnail image. Only valid for specific types. */
    mutable QImage m_thumbnail;
    /*! Image format. */
    mutable QImage::Format m_imageFormat;
    /*! Size of the image (in pixels). */
    mutable QSize m_size;

  protected:

    /*! Minimal size of texture dimension (in pixels). */
    static const int KMinimalSize = 1;
    /*! Maximal size of texture dimension (in pixels). */
    static const int KMaximalSize = 4096;
    /*! Minifying filtering property name. */
    static const QString KPropertyNameMinifyingFiltering;
    /*! Magnifying filtering property name. */
    static const QString KPropertyNameMagnifyingFiltering;
    /*! Mip maping filtering property name. */
    static const QString KPropertyNameMipMappingFiltering;
    /*! Addressing mode S property name. */
    static const QString KPropertyNameAddressingModeS;
    /*! Addressing mode T property name. */
    static const QString KPropertyNameAddressingModeT;
    /*! Location property name. */
    static const QString KPropertyNameLocation;
    /*! Width property name. */
    static const QString KPropertyNameWidth;
    /*! Height property name. */
    static const QString KPropertyNameHeight;
    /*! Image format property name. */
    static const QString KPropertyNameImageFormat;
    /*! Info group name. */
    static const QString KGroupNameInfo;
    /*! Filtering group name. */
    static const QString KGroupNameFiltering;
    /*! Addressing group name. */
    static const QString KGroupNameAddressing;

  private slots:

    /*! Slot called when internal (lazy) data should be invalidated. */
    virtual void onInvalidate();

  private:

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
    virtual void addAddressingModeDefinitions(NPropertyObject::PropertyDefinition& group) const;
    /*! Returns image format in string format. */
    QString imageFormatAsText() const;

    /*! Returns texture type. */
    TextureType type() const;
    /*! Sets minification filter. */
    void setMinificationFilter(TextureFilterType type);
    /*! Returns minification filter. */
    TextureFilterType minificationFilter() const;
    /*! Sets magnification filter. */
    void setMagnificationFilter(TextureFilterType type);
    /*! Returns magnification filter. */
    TextureFilterType magnificationFilter() const;
    /*! Sets mip mapping filter. */
    void setMipMappingFilter(TextureMipMappingFilterType type);
    /*! Returns mip mapping filter. */
    TextureMipMappingFilterType mipMappingFilter() const;
    /*! Returns addressing mode for S coordinate. */
    TextureAddressMode addressModeS() const;
    /*! Returns addressing mode for T coordinate. */
    TextureAddressMode addressModeT() const;

  private:

    /*! Full path to asset including file name. */
    QString m_fullPath;
    /*! Texture type. */
    TextureType m_type;
    /*! Magnification filter. */
    TextureFilterType m_magnificationFilter;
    /*! Minification filter. */
    TextureFilterType m_minificationFilter;
    /*! Mip Mapping filter. */
    TextureMipMappingFilterType m_mipMappingFilter;
    /*! Addressing mode for S coordinate. */
    TextureAddressMode m_addressingModeS;
    /*! Addressing mode for T coordinate. */
    TextureAddressMode m_addressingModeT;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // RESOURCELIBRARY_RESOURCEITEMTEXTURE_H
