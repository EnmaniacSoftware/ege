#ifndef RESOURCELIBRARY_RESOURCEITEMTEXTUREATLAS_H
#define RESOURCELIBRARY_RESOURCEITEMTEXTUREATLAS_H

/*! Specialization of resource item.
 *  Class representing texture atlas within Resource Library.
 */

#include <QImage>
#include "ResourceItemTexture.h"

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

    /*! Rebuilds atlas image. */
    void rebuild();

  private:

    /*! Texture image. */
    QImage m_image;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // RESOURCELIBRARY_RESOURCEITEMTEXTUREATLAS_H
