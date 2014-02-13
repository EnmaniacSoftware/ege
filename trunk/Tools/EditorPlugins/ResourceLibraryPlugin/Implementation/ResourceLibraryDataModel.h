#ifndef RESOURCELIBRARY_RESOURCELIBRARYDATAMODEL_H
#define RESOURCELIBRARY_RESOURCELIBRARYDATAMODEL_H

/*! Resource library data model class.
 *  Entire model's layout is 2D grid like with exactly one column.
 *  Each item corresponds to the row index which is based on the actual position within parent's children list.
 */

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <Serializer.h>
#include <QList>
#include <QMap>
#include <QUuid>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceItem;
class ResourceItemFactory;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceLibraryDataModel : public QAbstractItemModel
                               , public ISerializer
{
  Q_OBJECT

  public:

    explicit ResourceLibraryDataModel(QObject* parent = NULL);
   ~ResourceLibraryDataModel();

  public:

    /*! Available custom roles. */
    enum ResourceLibraryModelRole
    {
      TypeRole = Qt::UserRole,                  /*!< Resource item type. */
      PathRole,                                 /*!< Resource item path. Only valid for certain types. */
      ConfigRole                                /*!< Configuration item is bound to. */
    };

  signals:

    /*! Signal emitted when given item has been added to model. */
    void itemAdded(ResourceItem* item);
    /*! Signal emitted when given item has been removed from model. */
    void itemRemoved(ResourceItem* item);
    /*! Signal emitted when given item has changed. */
    void itemChanged(const ResourceItem* item);

  public:

    /*! Clears data. */
    void clear();

    /*! @see QAbstractItemModel::setData. */
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
    /*! @see QAbstractItemModel::index. */
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;

    /*! @see ISerializer::serialize. */
    bool serialize(QXmlStreamWriter& stream) const override;
    /*! @see ISerializer::unserialize. */
    bool unserialize(QXmlStreamReader& stream) override;

    /*! Removes entry associated with given index. */
    void removeItem(const QModelIndex& index);
    /*! Inserts item after given index. */
    QModelIndex insertItem(const QModelIndex& index, ResourceItem* item);

    /*! Returns TRUE if model is empty. */
    bool isEmpty() const;

    /*! @see QAbstractItemModel::rowCount. */
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    /*! Returns all resource items of a given type.
     *  @param  typeName  Name of the type of objects to return.
     *  @return List of pointers to objects of a given type currently present in the model.
     */
    QList<ResourceItem*> items(const QString& typeName) const;

    /*! Returns item with given ID.
     *  @param  id  ID of the item.
     *  @return Item with given ID. NULL if not found.
     */
    ResourceItem* item(const QUuid& id) const;

  private slots:

    /*! Slot called when current configuration name has changed.
     *  @param  oldName Old configuration name.
     *  @param  newName New configuration name.
     */
    void onConfigurationNameChanged(const QString& oldName, const QString& newName);
    /*! Slot called when configuration has been removed.
     *  @param  Name  Name of the configuration which has been removed.
     */
    void onConfigurationRemoved(const QString& name);

  private:

    /*! @see QAbstractItemModel::parent. */
    QModelIndex parent(const QModelIndex& index) const override;
    /*! @see QAbstractItemModel::data. */
    QVariant data(const QModelIndex& index, int role) const override;
    /*! @see QAbstractItemModel::flags. */
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    /*! @see QAbstractItemModel::columnCount. */
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    /*! Returns resource library item connected to a given model index. */
    ResourceItem* getItem(const QModelIndex& index) const;
    /*! Returns root item. */
    ResourceItem* root() const;
    /*! Returns list of all items in the model. */
    QList<ResourceItem*> allItems() const;
    /*! Adds children of given resource item to the list. */
    void addChildren(ResourceItem* item, QList<ResourceItem*>& list) const;
    /*! Recursively removes all children of a given configuration from the parent.
     *  @param  parent  Parent item from which children are to be removed.
     *  @param  name    Name of the configuration for which items are to be removed.
     */
    void removeChildItems(ResourceItem* parent, const QString& name);

  private:

    /*! Internal data type storing all resource items by ID. */
    typedef QMap<QUuid, ResourceItem*> UuidResourceItemMap;

  private:

    /*! Root item. */
    ResourceItem* m_root;
    /*! Map of all items sorted by ID. */
    UuidResourceItemMap m_uuidToItem;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // RESOURCELIBRARY_RESOURCELIBRARYDATAMODEL_H
