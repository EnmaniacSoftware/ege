CONFIG      += designer plugin

TARGET      = $$qtLibraryTarget($$TARGET)
TEMPLATE    = lib

HEADERS     = BlendModeSelectorWidget.h \
              BlendModeSelectorWidgetPlugin.h

SOURCES     = BlendModeSelectorWidget.cpp \
              BlendModeSelectorWidgetPlugin.cpp

target.path = $$[QT_INSTALL_PLUGINS]/designer

INSTALLS += target
