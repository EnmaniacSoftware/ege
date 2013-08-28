CONFIG      += plugin

TARGET      = $$qtLibraryTarget($$TARGET)
TEMPLATE    = lib

HEADERS     = ImagedAnimationPlugin.h \
              ImagedAnimationWindow.h

SOURCES     = ImagedAnimationPlugin.cpp \
              ImagedAnimationWindow.cpp

FORMS       = imagedanimationwindow.ui

INCLUDEPATH += ../../EGEEditor/src \
               "../../EGE Editor Plugins/CorePlugin" \
               "../../EGE Editor Plugins/ResourceLibraryPlugin"

DEFINES += override=""

INSTALLS += target
