include(ResourceLibraryPlugin.pri)
include(../PluginExtras.pri)

QT += widgets

INCLUDEPATH += Interface \
            ../CorePlugin/Interface \
            ../WorkspacePlugin/Interface

DEFINES += RESOUCELIBRARYPLUGIN_EXPORT=1

LIBS += -lWorkspacePlugin -lCorePlugin

target.path = ../../EGEEditor/Plugins
INSTALLS += target
