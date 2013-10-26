CONFIG      += plugin

QT          += widgets

TEMPLATE    = lib

HEADERS     = MaterialManagerPlugin.h \
              MaterialManagerWindow.h \
              ResourceItemMaterial.h

SOURCES     = MaterialManagerPlugin.cpp \
              MaterialManagerWindow.cpp \
              ResourceItemMaterial.cpp

FORMS       = MaterialManagerWindow.ui

INCLUDEPATH += ../../EGEEditor/Interface \
               ../../EditorPlugins/CorePlugin \
               ../../EditorPlugins/ResourceLibraryPlugin

DEFINES += override="" \
           MATERIALMANAGERPLUGIN_EXPORT

LIBS += -L$$PWD/../../bin -lCorePlugin -lResourceLibraryPlugin

DESTDIR = $$PWD/../../bin
