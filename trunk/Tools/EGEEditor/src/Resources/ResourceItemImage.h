#ifndef RESOURCE_ITEM_IMAGE_H
#define RESOURCE_ITEM_IMAGE_H

#include <QImage>
#include "ResourceItem.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceItemImage : public ResourceItem
{
  public:

    ResourceItemImage();
   ~ResourceItemImage();

  public:

    /* Creates instance of resource item. This method is a registration method for factory. */
    static ResourceItem* Create();

  public:

    /* Returns thumbnail image. 
     * @note Generates thumbnail image if required. 
     */
    const QImage& thumbnailImage() const;
    /* Sets path. */
    void setPath(const QString& path);
    /*! Returns path to asset. */
    inline const QString& path() const { return m_path; }

  private:

    /* ResourceItem override. Returns type name. */
    QString type() const override;
    /* ResourceItem override. Returns size hint. */
    QSize sizeHint() const override;
    /* ResourceItem override. Serializes into given stream. */
    bool serialize(QXmlStreamWriter& stream) const override;
    /* ResourceItem override. Unserializes from given data stream. */
    bool unserialize(const QXmlStreamReader& stream) override;
    /* ResourceItem override. Returns data for a given column and role. 
     * @param columnIndex Column index for which data is to be retrieved.
     * @param role        Role for which data is to be retrieved.
     * @return Returns data associated with a given role at given column. If no valid data is present returns empty QVariant.
     */
    QVariant data(int columnIndex, int role) const override;
    /* ResourceItem override. Sets the role data.
     *  @param value  Value to be set.
     *  @param role   Role for which data is set.
     *  @return Returns TRUE if data has been changed. Otherwise FALSE.
     */
    bool setData(const QVariant &value, int role) override;

  private:

    /*! Thumbnail image. Only valid for specific types. */
    mutable QImage m_thumbnail;
    /*! Path to asset. */
    QString m_path;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // RESOURCE_ITEM_IMAGE_H