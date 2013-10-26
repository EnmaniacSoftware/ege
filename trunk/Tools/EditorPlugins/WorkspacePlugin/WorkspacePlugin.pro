include(WorkspacePlugin.pri)
include(../PluginExtras.pri)

QT += core gui widgets

INCLUDEPATH += Interface \
               ../CorePlugin/Interface

DEFINES +=  WORKSPACEPLUGIN_EXPORT=1

LIBS += -lCorePlugin
