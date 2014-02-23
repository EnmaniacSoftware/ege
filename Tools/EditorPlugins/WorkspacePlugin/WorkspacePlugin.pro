include(WorkspacePlugin.pri)
include(../PluginExtras.pri)
include(../../External/QtSolutions/qtpropertybrowser/src/qtpropertybrowser.pri)

QT += core gui widgets

INCLUDEPATH += Interface \
               ../CorePlugin/Interface

DEFINES += WORKSPACEPLUGIN_EXPORT=1

LIBS += -lCorePlugin

target.path = ../../EGEEditor/Plugins
INSTALLS += target
