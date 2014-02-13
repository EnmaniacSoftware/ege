#ifndef RESOURCELIBRARY_RESOURCELIBRARY_H
#define RESOURCELIBRARY_RESOURCELIBRARY_H

/*! Resource library. This object contains a collection of resource items available to current project.
 */

#include <QObject>
#include <QItemSelection>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceLibraryDataModel;
class ResourceLibraryDataModelProxy;
class ResourceLibraryWindow;
class ResourceItem;
class QXmlStreamWriter;
class QXmlStreamReader;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceLibrary : public QObject
{
  Q_OBJECT

  public:

    explicit ResourceLibrary(ResourceLibraryWindow* window, QObject* parent = NULL);
   ~ResourceLibrary();

  signals:

    /*! Signal emitted when data loading has completed.
     *  @param  count Number of loaded items.
     */
    void loaded(int count);
    /*! Signal emitted when given item has been removed from model.
     *  @param  item  Item being removed.
     */
    void itemRemoved(ResourceItem* item);

  public:

    /*! Inserts given item into the model at current selection.
     *  @param  item  Item to be inserted into the model.
     */
    void insertItem(ResourceItem* item);
    /*! Removes currently selected items. */
    // TAGE - make it slot ?
    void removeSelectedItems();

    /*! Returns TRUE if model is empty.
     *  @note Test is done for current configuration only.
     */
    bool isEmpty() const;

    /*! Returns all resource items of a given type.
     *  @param  typeName  Name of the type of objects to return.
     *  @param  parent    If not NULL, only child resource of this object will be returned.
     *  @return List of pointers to objects of a given type currently present in the model.
     *  @note Only items belonging to current configuration are returned.
     */
    QList<ResourceItem*> items(const QString& typeName, const ResourceItem* parent = NULL) const;
    /*! Returns item with given ID.
     *  @param  id  ID of the item.
     *  @return Item with given ID. NULL if not found.
     */
    ResourceItem* item(const QUuid& id) const;

  private slots:

    /*! Slot called when library saving is required.
     *  @param  stream  XML stream where data is to be saved.
     */
    void onSaveData(QXmlStreamWriter& stream);
    /*! Slot called when library loading is required.
     *  @param  stream  XML stream where data is to be saved.
     */
    void onLoadData(QXmlStreamReader& stream);
    /*! Slot called when new object has been added into the pool. */
    void onObjectAdded(QObject* object);
    /*! Slot called when object is about to be removed from pool. */
    void onObjectRemoved(QObject* object);
    /*! Slot called when selection changes. */
    void onSelectionChanged(const QItemSelection& selectedItems, const QItemSelection& deselectedItems);
    /*! Slot called when currently selected item needs update. */
    void onUpdateSelection();

  private:

    /*! Returns proxy model. */
    ResourceLibraryDataModelProxy* proxyModel() const;
    /*! Returns model. */
    ResourceLibraryDataModel* model() const;

  private:

    /*! Data model. */
    ResourceLibraryDataModel* m_model;
    /*! Proxy model for filtering. */
    ResourceLibraryDataModelProxy* m_filterProxy;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // RESOURCELIBRARY_RESOURCELIBRARY_H
