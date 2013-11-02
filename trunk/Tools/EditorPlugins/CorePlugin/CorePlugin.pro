include(CorePlugin.pri)
include(../PluginExtras.pri)

QT += widgets

INCLUDEPATH += Interface

DEFINES += COREPLUGIN_EXPORT=1

target.path = ../../EGEEditor/Plugins
INSTALLS += target
