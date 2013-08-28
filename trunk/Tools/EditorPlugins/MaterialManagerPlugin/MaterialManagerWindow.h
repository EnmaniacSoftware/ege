#ifndef MATERIALMANAGERWINDOW_H
#define MATERIALMANAGERWINDOW_H

#include <QtWidgets/QDialog>
#include "Serializer.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Ui_MaterialManager;
class Config;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Material manager module window. */
class MaterialManagerWindow : public QDialog, public ISerializer
{
  Q_OBJECT

  public:

    explicit MaterialManagerWindow(QWidget* parent = NULL);
   ~MaterialManagerWindow();

    /* Saves settings. */
    void saveSettings(Config* config);
    /* Loads settings. */
    void loadSettings(Config* config);

    /* ISerializer override. Serializes into given stream. */
    bool serialize(QXmlStreamWriter& stream) const override;
    /* ISerializer override. Unserializes from given data stream. */
    bool unserialize(QXmlStreamReader& stream) override;

  private:

    /* Updates menus. */
    void updateMenus();
    /* Attaches resource library specific data. */
    void attachToResourceLibrary();

  private:

    /*! Material manager window UI. */
    Ui_MaterialManager* m_ui;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // MATERIALMANAGERWINDOW_H
