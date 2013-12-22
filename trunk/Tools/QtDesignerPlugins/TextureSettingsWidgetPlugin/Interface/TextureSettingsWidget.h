#ifndef TEXTURESETTINGS_TEXTURESETTINGSWIDGET_H
#define TEXTURESETTINGS_TEXTURESETTINGSWIDGET_H

#include <QWidget>
#include <QtDesigner/QDesignerExportWidget>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Ui_TextureSettings;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class QDESIGNER_WIDGET_EXPORT TextureSettingsWidget : public QWidget
{
  Q_OBJECT

  public:

    TextureSettingsWidget(QWidget *parent = NULL);

  private:

    /*! UI. */
    Ui_TextureSettings* m_ui;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // TEXTURESETTINGS_TEXTURESETTINGSWIDGET_H
