#CONFIG      += plugin

QT          += widgets

TARGET      = $$qtLibraryTarget($$TARGET)
TEMPLATE    = lib

HEADERS     = FontManagerPlugin.h \
#              FontManagerWindow.h \
              ResourceItemFont.h

SOURCES     = FontManagerPlugin.cpp \
#              FontManagerWindow.cpp \
              ResourceItemFont.cpp

INCLUDEPATH += ../../EGEEditor/src \
               ../../EditorPlugins/CorePlugin \
               ../../EditorPlugins/ResourceLibraryPlugin

DEFINES += override="" \
           FONTMANAGERPLUGIN_EXPORT=1

INSTALLS += target
