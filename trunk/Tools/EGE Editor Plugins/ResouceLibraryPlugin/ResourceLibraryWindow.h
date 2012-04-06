#ifndef RESOURCE_LIBRARY_WINDOW_H
#define RESOURCE_LIBRARY_WINDOW_H

#include <QDockWidget>
#include "Serializer.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Ui_ResourceLibrary;
class Config;
class MainWindow;
class ResourceLibraryDataModel;
class ResourceLibraryItemDelegate;
class Project;

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

    /*! Returns model. */
    ResourceLibraryDataModel* model() const { return m_model; }

  private slots:

    /* Slot called when context menu is requested. */
		void onQueueContextMenuRequested(const QPoint& pos);
    /* Slot called when new project has been created/opened. */
    void onProjectCreated(Project* project);
    /* Slot called when project has been closed. */
    void onProjectClosed();
    /* Slot called when container is requested to be added. */
    void onAddContainer();
    /* Slot called when resource is requested to be added. */
    void onAddResource();
    /* Slot called when resource items are requested to be removed. */
    void onRemoveItems();

  private:

    /* Updates menus. */
    void updateMenus();

  private:

    /*! Resource library window UI. */
    Ui_ResourceLibrary* m_ui;
    /*! Data model. */
    ResourceLibraryDataModel* m_model;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // RESOURCE_LIBRARY_WINDOW_H