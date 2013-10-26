include(WorkspacePlugin.pri)

TEMPLATE = lib

CONFIG += plugin

QT += core gui widgets

INCLUDEPATH += Interface \
               ../CorePlugin/Interface \
               ../../EGEEditor/Interface

Debug:UI_DIR   = debug/.ui
Release:UI_DIR = release/.ui

DEFINES += override="" \
           WORKSPACEPLUGIN_EXPORT=1

LIBS += -L$$PWD/../../bin -lCorePlugin

DESTDIR = $$PWD/../../bin
