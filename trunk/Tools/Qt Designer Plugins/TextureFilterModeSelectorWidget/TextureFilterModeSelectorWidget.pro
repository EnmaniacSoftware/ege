CONFIG      += designer plugin

TARGET      = $$qtLibraryTarget($$TARGET)
TEMPLATE    = lib

HEADERS     = TextureFilterModeSelectorWidget.h \
              TextureFilterModeSelectorWidgetPlugin.h

SOURCES     = TextureFilterModeSelectorWidget.cpp \
              TextureFilterModeSelectorWidgetPlugin.cpp

target.path = $$[QT_INSTALL_PLUGINS]/designer

INSTALLS += target
