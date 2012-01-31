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

    ResourceLibraryItem(ResourceLibraryItem* parent, const QString& name, const QString& path, Type type);
   ~ResourceLibraryItem();

    /* Returns number of columns. */
    int columnCount() const;
    /* Returns children count. */
    int childCount() const;
    /*! Returns parent. */
    inline ResourceLibraryItem* parent() const { return m_parent; }
    /* Returns row index at which current item is placed withing parent. */
    int row() const;
    /* Returns data for a given column. */
    QVariant data(int columnIndex) const;
    /* Adds child. */
    void add(ResourceLibraryItem* child);

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