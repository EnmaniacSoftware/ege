#ifndef RESOURCEITEM_MATERIAL_H
#define RESOURCEITEM_MATERIAL_H

/*! Class representing Material resource item. 
 */

#include <ResourceItem.h>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceItemMaterial : public ResourceItem
{
  public:

    ResourceItemMaterial();
   ~ResourceItemMaterial();

  public:

    /* Creates instance of resource item. This method is a registration method for factory. */
    static ResourceItem* Create();

  public:

  private:

    /* ResourceItem override. Returns type name. */
    QString type() const override;
    /* ResourceItem override. Returns size hint. */
    QSize sizeHint() const override;
    /* ResourceItem override. Serializes into given stream. */
    bool serialize(QXmlStreamWriter& stream) const override;
    /* ResourceItem override. Unserializes from given data stream. */
    bool unserialize(QXmlStreamReader& stream) override;
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

};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // RESOURCEITEM_MATERIAL_H