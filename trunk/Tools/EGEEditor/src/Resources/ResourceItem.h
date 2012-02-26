#ifndef RESOURCE_ITEM_H
#define RESOURCE_ITEM_H

#include <QList>
#include <QString>
#include <QVariant>
#include "Serializer.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Generic resource item. */
class ResourceItem : public ISerializer
{
  public:

    ResourceItem();
    virtual ~ResourceItem();

    /* Sets name. */
    void setName(const QString& name);
    /*! Returns name. */
    inline const QString& name() const { return m_name; }
    
    /* Returns type name. */
    virtual QString type() const;

    /* Sets parent. */
    void setParent(ResourceItem* parent);
    /*! Returns parent. */
    inline ResourceItem* parent() const { return m_parent; }

    /* Sets path. */
    void setPath(const QString& path);
    /*! Returns path to asset. */
    inline const QString& path() const { return m_path; }

    /* Returns number of columns. */
    int columnCount() const;

    /* Returns children count. */
    int childCount() const;
    /* Returns child at given index. */
    ResourceItem* child(int index) const;
    /* Sets child item. */
    void setChild(int index, ResourceItem* item);
    
    /* Returns row index at which current item is placed withing parent. */
    int row() const;
    /* Returns data for a given column and role. 
     * @param columnIndex Column index for which data is to be retrieved.
     * @param role        Role for which data is to be retrieved.
     * @return Returns data associated with a given role at given column. If no valid data is present returns empty QVariant.
     */
    QVariant data(int columnIndex, int role) const;
    /* Sets the role data.
     *  @param value  Value to be set.
     *  @param role   Role for which data is set.
     *  @return Returns TRUE if data has been changed. Otherwise FALSE.
     */
    bool setData(const QVariant &value, int role);
    /* Inserts children. */
    bool insertChildren(int position, int count, int columns);

    /* Returns the item flags for the given item. */
    virtual Qt::ItemFlags flags() const;

    /* ISerializer override. Serializes into given buffer. */
    QString serialize() const override;
    /* ISerializer override. Unserializes from given data buffer. */
    bool unserialize(const QString& data) override;

  protected:

    /*! List of children. */
    QList<ResourceItem*> m_children;
    /*! Name of asset. */
    QString m_name;
    /*! Path to asset. Only valid for specific types. */
    QString m_path;

  private:

    /*! Parent. */
    ResourceItem* m_parent;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // RESOURCE_ITEM_H