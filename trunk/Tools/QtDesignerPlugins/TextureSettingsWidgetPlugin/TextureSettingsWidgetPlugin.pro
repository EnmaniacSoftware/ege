include(TextureSettingsWidgetPlugin.pri)
include(../PluginExtras.pri)

INCLUDEPATH += Interface

target.path += $$[QT_INSTALL_PLUGINS]/designer \
#target.path += $$[QT_INSTALL_PREFIX]/../../Tools/QtCreator/bin/plugins/designer

INSTALLS += target
