include(../Extras.pri)

# common setup for all plugins
TEMPLATE = lib

CONFIG += plugin

INCLUDEPATH += ../../EGEEditor/Interface

DESTDIR = $$PWD/../bin

LIBS += -L$$PWD/../bin

Debug:UI_DIR   = debug/.ui
Release:UI_DIR = release/.ui
