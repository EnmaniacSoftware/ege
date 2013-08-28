CONFIG      += plugin

QT          += widgets

TARGET      = $$qtLibraryTarget($$TARGET)
TEMPLATE    = lib

HEADERS     = ImagedAnimationPlugin.h \
              ImagedAnimationWindow.h

SOURCES     = ImagedAnimationPlugin.cpp \
              ImagedAnimationWindow.cpp

FORMS       = imagedanimationwindow.ui

INCLUDEPATH += ../../EGEEditor/src \
               ../../EditorPlugins/CorePlugin \
               ../../EditorPlugins/ResourceLibraryPlugin

DEFINES += override="" IMAGEDANIMATIONPLUGIN_EXPORT

INSTALLS += target
