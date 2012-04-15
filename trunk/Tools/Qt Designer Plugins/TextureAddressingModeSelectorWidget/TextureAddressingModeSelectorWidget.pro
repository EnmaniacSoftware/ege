CONFIG      += designer plugin

TARGET      = $$qtLibraryTarget($$TARGET)
TEMPLATE    = lib

HEADERS     = TextureAddressingModeSelectorWidget.h \
              TextureAddressingModeSelectorWidgetPlugin.h

SOURCES     = TextureAddressingModeSelectorWidget.cpp \
              TextureAddressingModeSelectorWidgetPlugin.cpp

target.path = $$[QT_INSTALL_PLUGINS]/designer

INSTALLS += target
