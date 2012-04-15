#ifndef BLENDMODESELECTORWIDGET_H
#define BLENDMODESELECTORWIDGET_H

#include <QComboBox>
#include <QtDesigner/QDesignerExportWidget>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class QDESIGNER_WIDGET_EXPORT BlendModeSelectorWidget : public QComboBox
{
  Q_OBJECT

  public:

    BlendModeSelectorWidget(QWidget *parent = NULL);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // BLENDMODESELECTORWIDGET_H
