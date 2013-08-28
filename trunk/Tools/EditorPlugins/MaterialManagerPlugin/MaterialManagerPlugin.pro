CONFIG      += plugin

QT          += widgets

TARGET      = $$qtLibraryTarget($$TARGET)
TEMPLATE    = lib

HEADERS     = MaterialManagerPlugin.h \
              MaterialManagerWindow.h \
              ResourceItemMaterial.h

SOURCES     = MaterialManagerPlugin.cpp \
              MaterialManagerWindow.cpp \
              ResourceItemMaterial.cpp

FORMS       = MaterialManagerWindow.ui

INCLUDEPATH += ../../EGEEditor/src \
               ../../EditorPlugins/CorePlugin \
               ../../EditorPlugins/ResourceLibraryPlugin

DEFINES += override="" MATERIALMANAGERPLUGIN_EXPORT

INSTALLS += target
