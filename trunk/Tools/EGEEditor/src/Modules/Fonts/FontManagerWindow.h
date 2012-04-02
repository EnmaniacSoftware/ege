#ifndef FONTMANAGERWINDOW_H
#define FONTMANAGERWINDOW_H

#include <QDialog>
#include "Serializer.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Ui_FontManager;
class Config;
class MainWindow;
class Project;
class IResourceLibraryDataModel;
class ResourceItem;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Fonts library window. */
class FontManagerWindow : public QDialog, public ISerializer
{
  Q_OBJECT

  public:

    FontManagerWindow(IResourceLibraryDataModel* resourceDataModel);
   ~FontManagerWindow();

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

    /* Slot called when item has been added to resource model. */
    void onResourceModelItemAdded(ResourceItem* item);
    /* Slot called when item has been removed from resource model. */
    void onResourceModelItemRemoved(ResourceItem* item);
    /* Slot called when bitmap font selection changes. */
    void onFontBitmapChanged(int index);
    /* Slot called when title should be updated. */
    void updateTitle();

  private:

    /* QDialog override. Receives events to an object and should return true if the event was recognized and processed. */
    bool event(QEvent* event);
    /* Window has been activated. */
    void activated();
    /* Sets action menu. */
    void setActionMenu(bool enable);

  private:

    /*! UI. */
    Ui_FontManager* m_ui;
    /*! Font image. */
    QImage m_image;
    /*! Data model. */
    //ResourceLibraryDataModel* m_model;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // FONTMANAGERWINDOW_H