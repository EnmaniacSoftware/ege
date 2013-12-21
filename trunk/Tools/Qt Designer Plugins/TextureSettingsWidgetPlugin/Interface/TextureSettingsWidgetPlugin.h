#ifndef TEXTURESETTINGS_TEXTURESETTINGSWIDGETPLUGIN_H
#define TEXTURESETTINGS_TEXTURESETTINGSWIDGETPLUGIN_H

#include <QDesignerCustomWidgetInterface>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class TextureSettingsWidgetPlugin : public QObject, public QDesignerCustomWidgetInterface
{
  Q_OBJECT
  Q_INTERFACES(QDesignerCustomWidgetInterface)
  Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QDesignerCustomWidgetInterface" FILE "texturesettingswidget.json")

  public:

    TextureSettingsWidgetPlugin(QObject* parent = NULL);

    bool isContainer() const;
    bool isInitialized() const;
    QIcon icon() const;
    QString domXml() const;
    QString group() const;
    QString includeFile() const;
    QString name() const;
    QString toolTip() const;
    QString whatsThis() const;
    QWidget *createWidget(QWidget* parent);
    void initialize(QDesignerFormEditorInterface* core);

  private:
  
    /*! Initialize flag. */
    bool m_initialized;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // TEXTURESETTINGS_TEXTURESETTINGSWIDGETPLUGIN_H
