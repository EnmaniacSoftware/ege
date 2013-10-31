include(JuiceJumpersProjectPlugin.pri)
include(../PluginExtras.pri)

INCLUDEPATH += ../CorePlugin/Interface

DEFINES += JUICEJUMPERSPROJECTPLUGIN_EXPORT=1

LIBS += -lCorePlugin

