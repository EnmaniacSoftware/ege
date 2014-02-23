#ifndef TEXTUREENVIRONMENTMODESELECTORWIDGET_H
#define TEXTUREENVIRONMENTMODESELECTORWIDGET_H

#include <QComboBox>
#include <QtDesigner/QDesignerExportWidget>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class QDESIGNER_WIDGET_EXPORT TextureEnvironmentModeSelectorWidget : public QComboBox
{
  Q_OBJECT

  public:

    TextureEnvironmentModeSelectorWidget(QWidget *parent = NULL);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // TEXTUREENVIRONMENTMODESELECTORWIDGET_H
