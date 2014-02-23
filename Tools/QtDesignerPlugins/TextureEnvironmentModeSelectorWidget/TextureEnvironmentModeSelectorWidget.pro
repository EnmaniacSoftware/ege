CONFIG      += designer plugin

TARGET      = $$qtLibraryTarget($$TARGET)
TEMPLATE    = lib

HEADERS     = TextureEnvironmentModeSelectorWidget.h \
              TextureEnvironmentModeSelectorWidgetPlugin.h

SOURCES     = TextureEnvironmentModeSelectorWidget.cpp \
              TextureEnvironmentModeSelectorWidgetPlugin.cpp

target.path = $$[QT_INSTALL_PLUGINS]/designer

INSTALLS += target
