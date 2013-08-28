CONFIG      += plugin

TARGET      = $$qtLibraryTarget($$TARGET)
TEMPLATE    = lib

HEADERS     = FontManagerPlugin.h \
#              FontManagerWindow.h \
              ResourceItemFont.h

SOURCES     = FontManagerPlugin.cpp \
#              FontManagerWindow.cpp \
              ResourceItemFont.cpp

INCLUDEPATH += ../../EGEEditor/src \
               "../../EGE Editor Plugins/CorePlugin" \
               "../../EGE Editor Plugins/ResourceLibraryPlugin"

DEFINES += override=""

INSTALLS += target
