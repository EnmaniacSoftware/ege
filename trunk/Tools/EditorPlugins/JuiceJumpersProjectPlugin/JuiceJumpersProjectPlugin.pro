include(JuiceJumpersProjectPlugin.pri)
include(../../Extras.pri)

TEMPLATE = lib

CONFIG += plugin

INCLUDEPATH += Interface \
               ../CorePlugin/Interface \
               ../../EGEEditor/Interface

win32
{
}
DEFINES += JUICEJUMPERSPROJECTPLUGIN_EXPORT

LIBS += -L$$PWD/../../bin -lCorePlugin

DESTDIR = $$PWD/../../bin
