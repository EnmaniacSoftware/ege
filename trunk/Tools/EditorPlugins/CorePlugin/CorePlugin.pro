include(CorePlugin.pri)
include(../PluginExtras.pri)

TEMPLATE = lib

CONFIG += plugin

QT += widgets

INCLUDEPATH += Interface \
               ../../EGEEditor/Interface

DEFINES += COREPLUGIN_EXPORT=1

DESTDIR = $$PWD/../../bin
