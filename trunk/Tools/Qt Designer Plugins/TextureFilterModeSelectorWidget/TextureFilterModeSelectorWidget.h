#ifndef TEXTUREFILTERMODESELECTORWIDGET_H
#define TEXTUREFILTERMODESELECTORWIDGET_H

#include <QComboBox>
#include <QtDesigner/QDesignerExportWidget>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class QDESIGNER_WIDGET_EXPORT TextureFilterModeSelectorWidget : public QComboBox
{
  Q_OBJECT

  public:

    TextureFilterModeSelectorWidget(QWidget *parent = NULL);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // TEXTUREFILTERMODESELECTORWIDGET_H
