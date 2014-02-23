#ifndef IMAGEDANIMATIONWINDOW_H
#define IMAGEDANIMATIONWINDOW_H

/*! Imaged animation module window. */

#include <QtWidgets/QDialog>
#include "Serializer.h"
#include "imagedanimationplugin_global.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Ui_ImagedAnimation;
class Config;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class IMAGEDANIMATIONPLUGIN_API ImagedAnimationWindow : public QDialog, public ISerializer
{
  Q_OBJECT

  public:

    explicit ImagedAnimationWindow(QWidget* parent = NULL);
   ~ImagedAnimationWindow();

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

  private:

    /*! Imaged animation window UI. */
    Ui_ImagedAnimation* m_ui;
    /*! SWFMILL XML to EGE framework XML converter. */
    //SwfMillToEgeConverter m_converter;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // IMAGEDANIMATIONWINDOW_H
