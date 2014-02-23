#ifndef TEXTUREENVIRONMENTMODESELECTORWIDGETPLUGIN_H
#define TEXTUREENVIRONMENTMODESELECTORWIDGETPLUGIN_H

#include <QDesignerCustomWidgetInterface>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class TextureEnvironmentModeSelectorWidgetPlugin : public QObject, public QDesignerCustomWidgetInterface
{
  Q_OBJECT
  Q_INTERFACES(QDesignerCustomWidgetInterface)

  public:

    TextureEnvironmentModeSelectorWidgetPlugin(QObject *parent = NULL);

    bool isContainer() const;
    bool isInitialized() const;
    QIcon icon() const;
    QString domXml() const;
    QString group() const;
    QString includeFile() const;
    QString name() const;
    QString toolTip() const;
    QString whatsThis() const;
    QWidget *createWidget(QWidget *parent);
    void initialize(QDesignerFormEditorInterface *core);

  private:
  
    /*! Initialize flag. */
    bool m_initialized;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // TEXTUREENVIRONMENTMODESELECTORWIDGETPLUGIN_H
