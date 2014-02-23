include(TextureSettingsWidgetPlugin.pri)
include(../PluginExtras.pri)
include(../../External/QtSolutions/qtpropertybrowser/src/qtpropertybrowser.pri)

INCLUDEPATH += Interface

target.path += $$[QT_INSTALL_PLUGINS]/designer \
#target.path += $$[QT_INSTALL_PREFIX]/../../Tools/QtCreator/bin/plugins/designer

INSTALLS += target
