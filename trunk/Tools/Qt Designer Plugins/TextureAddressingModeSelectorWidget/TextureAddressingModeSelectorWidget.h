#ifndef TEXTUREADDRESSINGMODESELECTORWIDGET_H
#define TEXTUREADDRESSINGMODESELECTORWIDGET_H

#include <QComboBox>
#include <QtDesigner/QDesignerExportWidget>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class QDESIGNER_WIDGET_EXPORT TextureAddressingModeSelectorWidget : public QComboBox
{
  Q_OBJECT

  public:

    TextureAddressingModeSelectorWidget(QWidget *parent = NULL);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // TEXTUREADDRESSINGMODESELECTORWIDGET_H
