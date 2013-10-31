#ifndef RESOURCELIBRARY_RESOURCEITEMIMAGE_H
#define RESOURCELIBRARY_RESOURCEITEMIMAGE_H

#include <QImage>
#include "ResourceItem.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceItemImage : public ResourceItem
{
  public:

    ResourceItemImage();
   ~ResourceItemImage();

  public:

    /*! Creates instance of resource item. This method is a registration method for factory. */
    static ResourceItem* Create();

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
