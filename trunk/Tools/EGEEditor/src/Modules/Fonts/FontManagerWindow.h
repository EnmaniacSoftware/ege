#ifndef FONTMANAGERWINDOW_H
#define FONTMANAGERWINDOW_H

#include <QMdiSubWindow>
#include "Serializer.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Ui_FontManager;
class Config;
class MainWindow;
class Project;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Fonts library window. */
class FontManagerWindow : public QMdiSubWindow, public ISerializer
{
  Q_OBJECT

  public:

    FontManagerWindow();
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

  private:

    /* Event called on application close request. */
    void closeEvent(QCloseEvent *event);

  private:

    /*! UI. */
    Ui_FontManager* m_ui;
    /*! Data model. */
    //ResourceLibraryDataModel* m_model;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // FONTMANAGERWINDOW_H