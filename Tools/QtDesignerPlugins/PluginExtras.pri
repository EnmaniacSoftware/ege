include(../Extras.pri)

# common setup for all plugins
TEMPLATE = lib

CONFIG += plugin release debug

QT     += widgets designer

DESTDIR = $$PWD/../bin

Debug:UI_DIR   = debug/.ui
Release:UI_DIR = release/.ui
