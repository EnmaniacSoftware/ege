#ifndef RESOURCELIBRARY_RESOURCEITEM_H
#define RESOURCELIBRARY_RESOURCEITEM_H

/*! Generic resource item. This is the base for all resource items. */

#include <QList>
#include <QString>
#include <QVariant>
#include <QSize>
#include <Serializer.h>
#include "ResouceLibraryPlugin_global.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class RESOUCELIBRARYPLUGIN_API ResourceItem : public QObject, public ISerializer
{
  Q_OBJECT

  public:

    ResourceItem();
    ResourceItem(const QString& name, ResourceItem* parent);
    virtual ~ResourceItem();

    /*! Sets name. */
    void setName(const QString& name);
    /*! Returns name. */
    const QString& name() const;
    
    /*! Returns type name. */
    virtual QString type() const;

    /*! Sets parent. */
    void setParent(ResourceItem* parent);
    /*! Returns parent. */
    ResourceItem* parent() const;

    /*! Returns number of columns. */
    int columnCount() const;

    /*! Returns children count. */
    int childCount() const;

    /*! Returns child at given index. */
    ResourceItem* child(int index) const;
    /*! Returns index of a given child. */
    int childIndex(ResourceItem* item) const;
    /*! Adds child to the end of pool. */
    void addChild(ResourceItem* item);

    /*! Sets child item. */
    void setChild(int index, ResourceItem* item);
    /*! Removes given child. */
    void removeChild(ResourceItem* item);
    /*! Removes all children. */
    void removeChildren();

    /*! Returns row index at which current item is placed within parent. */
    int row() const;
    /*! Returns data for a given column and role.
     *  @param columnIndex Column index for which data is to be retrieved.
     *  @param role        Role for which data is to be retrieved.
     *  @return Returns data associated with a given role at given column. If no valid data is present returns empty QVariant.
     */
    virtual QVariant data(int columnIndex, int role) const;
    /*! Sets the role data.
     *  @param value  Value to be set.
     *  @param role   Role for which data is set.
     *  @return Returns TRUE if data has been changed. Otherwise FALSE.
     */
    virtual bool setData(const QVariant &value, int role);
    /*! Inserts children. */
    bool insertChildren(int position, int count, int columns);

    /*! Returns the item flags for the given item. */
    virtual Qt::ItemFlags flags() const;

    /*! Returns size hint. */
    virtual QSize sizeHint() const;

    /*! @see ISerializer::serialize. */
    bool serialize(QXmlStreamWriter& stream) const override;
    /*! @see ISerializer::unserialize. */
    bool unserialize(QXmlStreamReader& stream) override;

  protected:

    /*! Begins serialization.
     *  @param  stream  Data stream serialization is to be done to.
     *  @return TRUE on success.
     *  @note This method begins serialization of any derived class. It must be called before any item can be properly serialized.
     */
    bool beginSerialize(QXmlStreamWriter& stream) const;
    /*! Ends serialization.
     *  @param  stream  Data stream serialization is to be done to.
     *  @return TRUE on success.
     *  @note This method ends serialization of any derived class. It must be called after item has been serialized.
     */
    bool endSerialize(QXmlStreamWriter& stream) const;

  protected:

    /*! List of children. */
    QList<ResourceItem*> m_children;
    /*! Name of asset. */
    QString m_name;

  private:

    /*! Parent. */
    ResourceItem* m_parent;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // RESOURCELIBRARY_RESOURCEITEM_H
