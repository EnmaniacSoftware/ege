#ifndef IMAGEDANIMATIONWINDOW_H
#define IMAGEDANIMATIONWINDOW_H

#include <QDialog>
#include "Serializer.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Ui_ImagedAnimation;
class Config;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Imaged animation module window. */
class ImagedAnimationWindow : public QDialog, public ISerializer
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
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // IMAGEDANIMATIONWINDOW_H