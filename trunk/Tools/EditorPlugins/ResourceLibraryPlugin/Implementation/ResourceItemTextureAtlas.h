#ifndef RESOURCELIBRARY_RESOURCEITEMTEXTUREATLAS_H
#define RESOURCELIBRARY_RESOURCEITEMTEXTUREATLAS_H

/*! Specialization of resource item.
 *  Class representing texture atlas within Resource Library.
 */

#include <QImage>
#include "ResourceItemTexture.h"
#include "ImageCompressionFormats.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class QMenu;
class QImage;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceItemTextureAtlas : public ResourceItemTexture
{
  Q_OBJECT

  public:

   ~ResourceItemTextureAtlas();

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

    /*! @see ResourceItemTexture::thumbnailImage. */
    const QImage& thumbnailImage() const override;
    /*! Sets size (in pixels). */
    void setSize(const QSize& size);
    /*! Sets image format. */
    void setImageFormat(QImage::Format format);

  private:

    ResourceItemTextureAtlas(const QString& name, const QString& configurationName, ResourceItem* parent);

    /*! @see ResourceItem::typeName. */
    QString typeName() const override;
    /*! @see ResourceItemTexture::size. */
    QSize size() const override;
    /*! @see ResourceItemTexture::onInvalidate. */
    void onInvalidate() override;
    /*! @see ResourceItemTexture::propertiesDefinition. */
    QList<NPropertyObject::PropertyDefinition> propertiesDefinition() const override;
    /*! @see ResourceItemTexture::update. */
    void update(const QString& name, const QVariant& value) override;
    /*! @see ResourceItemTexture::addAddressingModeDefinitions. */
    void addAddressingModeDefinitions(NPropertyObject::PropertyDefinition& group) const override;
    /*! Adds compression format definitions to given group.
     *  @param  group Definition group to which compression formats are to be added.
     */
    void addCompressionFormatDefinitions(NPropertyObject::PropertyDefinition& group) const;
    /*! Rebuilds atlas image. */
    void rebuild();
    /*! Creates texture format definition property.
     *  @return Property definition for texture formats.
     */
    NPropertyObject::PropertyDefinition createTextureFormatsDefinition() const;
    /*! Sets compression format. */
    void setCompressionFormat(ImageCompressionFormat format);
    /*! Returns compression format. */
    ImageCompressionFormat compressionFormat() const;

  private:

    /*! Compression property name. */
    static const QString KPropertyNameCompression;

  private:

    /*! Texture image. */
    QImage m_image;
    /*! Compression format. */
    ImageCompressionFormat m_compressionFormat;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // RESOURCELIBRARY_RESOURCEITEMTEXTUREATLAS_H
