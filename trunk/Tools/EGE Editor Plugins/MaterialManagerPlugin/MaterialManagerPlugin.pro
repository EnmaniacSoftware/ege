CONFIG      += plugin

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
               "../../EGE Editor Plugins/CorePlugin" \
               "../../EGE Editor Plugins/ResourceLibraryPlugin"

DEFINES += override=""

INSTALLS += target
