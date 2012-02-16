#ifndef RESOURCE_LIBRARY_ITEM_H
#define RESOURCE_LIBRARY_ITEM_H

#include <QList>
#include <QString>
#include <QVariant>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceLibraryItem
{
  public:

    /*! Available types. */
    enum Type
    {
      TYPE_NONE,
      TYPE_CONTAINER,
      TYPE_IMAGE,
      TYPE_SOUND
    };

  public:

    ResourceLibraryItem(ResourceLibraryItem* parent = NULL);
   ~ResourceLibraryItem();

    /* Sets name. */
    void setName(const QString& name);
    /*! Returns name. */
    inline const QString& name() const { return m_name; }
    /* Sets type. */
    void setType(Type type);
    /*! Returns item type. */
    inline Type type() const { return m_type; }

    /* Returns number of columns. */
    int columnCount() const;
    /* Returns children count. */
    int childCount() const;
    /* Returns child at given index. */
    ResourceLibraryItem* child(int index) const;
    /*! Returns parent. */
    inline ResourceLibraryItem* parent() const { return m_parent; }
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

  private:

    /*! Parent. */
    ResourceLibraryItem* m_parent;
    /*! List of children. */
    QList<ResourceLibraryItem*> m_children;
    /*! Name of asset. */
    QString m_name;
    /*! Path to asset. */
    QString m_path;
    /*! Asset type. */
    Type m_type;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // RESOURCE_LIBRARY_ITEM_H