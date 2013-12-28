#ifndef RESOURCELIBRARY_RESOURCELIBRARY_H
#define RESOURCELIBRARY_RESOURCELIBRARY_H

/*! Resource library. This object contains a collection of resource items available to current project.
 */

#include <QObject>

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
