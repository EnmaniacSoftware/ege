#ifndef FONTS_LIBRARY_H
#define FONTS_LIBRARY_H

#include <QDockWidget>
#include "Serializer.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Ui_FontsLibrary;
class Config;
class MainWindow;
class ResourceLibraryDataModel;
class ResourceLibraryItemDelegate;
class Project;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Fonts library window. */
class FontsLibraryWindow : public QDockWidget, public ISerializer
{
  Q_OBJECT

  public:

    FontsLibraryWindow(QWidget* parent);
   ~FontsLibraryWindow();

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

    /*! Returns pointer to main window object. */
    inline MainWindow* mainWindow() const { return reinterpret_cast<MainWindow*>(parent()); }

  private:

    /*! Fonts library window UI. */
    Ui_FontsLibrary* m_ui;
    /*! Data model. */
    //ResourceLibraryDataModel* m_model;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // FONTS_LIBRARY_H