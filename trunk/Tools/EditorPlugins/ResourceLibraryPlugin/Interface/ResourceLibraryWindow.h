#ifndef RESOURCELIBRARY_RESOURCELIBRARYWINDOW_H
#define RESOURCELIBRARY_RESOURCELIBRARYWINDOW_H

/*! Resource library window. */

#include <QDockWidget>
#include <QModelIndex>
#include <QAbstractItemView>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Ui_ResourceLibrary;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceLibraryWindow : public QDockWidget
{
  Q_OBJECT

  public:

    explicit ResourceLibraryWindow(QWidget* parent = NULL);
   ~ResourceLibraryWindow();

    /*! Saves settings. */
    void saveSettings();
    /*! Loads settings. */
    void loadSettings();

    /*! Returns list of selected indicies.
     *  @note List contains no duplicates and is not sorted.
     */
    QModelIndexList selectedIndexes() const;

    /*! Returns pointer to model view. */
    QAbstractItemView* view() const;

  public slots:

    /*! Slot called when model data has changed. */
    void onModelChanged();

  private slots:

    /*! Slot called when context menu is requested. */
		void onQueueContextMenuRequested(const QPoint& pos);
    /*! Slot called when resource items are requested to be removed. */
    void onRemoveItems();
    /*! Slot called when new object has been added into the pool. */
    void onObjectAdded(QObject* object);
    /*! Slot called when object is about to be removed from pool. */
    void onObjectRemoved(QObject* object);
    /*! Slot called when library data has been loaded.
     *  @param  count Number of items in library.
     */
    void onLibraryLoaded(int count);

  private:

    /*! Updates menus. */
    void updateMenus();

  private:

    /*! UI. */
    Ui_ResourceLibrary* m_ui;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // RESOURCELIBRARY_RESOURCELIBRARYWINDOW_H
