include(JuiceJumpersProjectPlugin.pri)
include(../PluginExtras.pri)

QT += widgets

INCLUDEPATH += ../CorePlugin/Interface \
               ../WorkspacePlugin/Interface \
               ../ResourceLibraryPlugin/Interface

DEFINES += JUICEJUMPERSPROJECTPLUGIN_EXPORT=1

LIBS += -lCorePlugin -lWorkspacePlugin -lResourceLibraryPlugin

target.path = ../../EGEEditor/Plugins
INSTALLS += target
