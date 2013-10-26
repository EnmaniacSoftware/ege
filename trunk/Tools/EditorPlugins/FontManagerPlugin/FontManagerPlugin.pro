CONFIG      += plugin

QT          += widgets

TEMPLATE    = lib

HEADERS     = FontManagerPlugin.h \
#              FontManagerWindow.h \
              ResourceItemFont.h

SOURCES     = FontManagerPlugin.cpp \
#              FontManagerWindow.cpp \
              ResourceItemFont.cpp

INCLUDEPATH += ../../EGEEditor/Interface \
               ../../EditorPlugins/CorePlugin \
               ../../EditorPlugins/ResourceLibraryPlugin

DEFINES += override="" \
           FONTMANAGERPLUGIN_EXPORT=1

LIBS += -L$$PWD/../../bin -lCorePlugin -lResourceLibraryPlugin

DESTDIR = $$PWD/../../bin
