#ifndef RESOURCELIBRARY_RESOURCELIBRARYWINDOW_H
#define RESOURCELIBRARY_RESOURCELIBRARYWINDOW_H

/*! Resource library window. */

#include <QDockWidget>
#include <Serializer.h>
#include "ui_ResourceLibraryWindow.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceLibraryDataModel;
class Ui_ResourceLibrary;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceLibraryWindow : public QDockWidget, public ISerializer
{
  Q_OBJECT

  public:

    explicit ResourceLibraryWindow(QWidget* parent = NULL);
   ~ResourceLibraryWindow();

    /*! Saves settings. */
    void saveSettings();
    /*! Loads settings. */
    void loadSettings();

    /*! @see ISerializer::serialize. */
    bool serialize(QXmlStreamWriter& stream) const override;
    /*! @see ISerializer::unserialize. */
    bool unserialize(QXmlStreamReader& stream) override;

    /*! Returns model. */
    ResourceLibraryDataModel* model() const;

  private slots:

    /*! Slot called when context menu is requested. */
		void onQueueContextMenuRequested(const QPoint& pos);
    /*! Slot called when container is requested to be added. */
    void onAddContainer();
    /*! Slot called when resource is requested to be added. */
    void onAddResource();
    /*! Slot called when resource items are requested to be removed. */
    void onRemoveItems();
    /*! Slot called when new object has been added into the pool. */
    void onObjectAdded(QObject* object);
    /*! Slot called when object is about to be removed from pool. */
    void onObjectRemoved(QObject* object);

  private:

    /*! Updates menus. */
    void updateMenus();

  private:

    /*! UI. */
    Ui_ResourceLibrary* m_ui;
    /*! Data model. */
    ResourceLibraryDataModel* m_model;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // RESOURCELIBRARY_RESOURCELIBRARYWINDOW_H
