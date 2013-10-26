include(CorePlugin.pri)

TEMPLATE = lib

CONFIG += plugin

QT += widgets

INCLUDEPATH += Interface \
               ../../EGEEditor/Interface

DEFINES += override="" \
           COREPLUGIN_EXPORT=1

DESTDIR = $$PWD/../../bin
