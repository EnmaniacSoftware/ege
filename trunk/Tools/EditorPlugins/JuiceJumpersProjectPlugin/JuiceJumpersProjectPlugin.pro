include(JuiceJumpersProjectPlugin.pri)
include(../PluginExtras.pri)

INCLUDEPATH += ../CorePlugin/Interface \
               ../../EGEEditor/Interface

DEFINES += JUICEJUMPERSPROJECTPLUGIN_EXPORT

LIBS += -lCorePlugin

