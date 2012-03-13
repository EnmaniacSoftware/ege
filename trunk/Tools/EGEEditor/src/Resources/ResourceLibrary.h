#ifndef RESOURCE_LIBRARY_H
#define RESOURCE_LIBRARY_H

#include <QDockWidget>
#include "Serializer.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Ui_ResourceLibrary;
class Config;
class MainWindow;
class ResourceLibraryDataModel;
class ResourceLibraryItemDelegate;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Resource library window. */
class ResourceLibraryWindow : public QDockWidget, public ISerializer
{
  Q_OBJECT

  public:

    ResourceLibraryWindow(QWidget* parent);
   ~ResourceLibraryWindow();

    /* Saves settings. */
    void saveSettings(Config* config);
    /* Loads settings. */
    void loadSettings(Config* config);

    /* ISerializer override. Serializes into given stream. */
    bool serialize(QXmlStreamWriter& stream) const override;
    /* ISerializer override. Unserializes from given data stream. */
    bool unserialize(QXmlStreamReader& stream) override;

  private slots:

    /* Slot called when context menu is requested. */
		void onQueueContextMenuRequested(const QPoint& pos);
    /* Slot called when new project has been created/opened. */
    void onProjectCreated();
    /* Slot called when project has been closed. */
    void onProjectClosed();
    /* Slot called when container is requested to be added. */
    void addContainer();
    /* Slot called when resource is requested to be added. */
    void addResource();
    /* Slot called when resource item is requested to be removed. */
    void removeItem();

  private:

    /*! Returns pointer to main window object. */
    inline MainWindow* mainWindow() const { return reinterpret_cast<MainWindow*>(parent()); }

  private:

    /*! Resource library window UI. */
    Ui_ResourceLibrary* m_ui;
    /*! Data model. */
    ResourceLibraryDataModel* m_model;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // RESOURCE_LIBRARY_H