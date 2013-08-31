CONFIG      += plugin

QT          += widgets

TEMPLATE    = lib

HEADERS     = ImagedAnimationPlugin.h \
              ImagedAnimationWindow.h

SOURCES     = ImagedAnimationPlugin.cpp \
              ImagedAnimationWindow.cpp

FORMS       = imagedanimationwindow.ui

INCLUDEPATH += ../../EGEEditor/src \
               ../../EditorPlugins/CorePlugin \
               ../../EditorPlugins/ResourceLibraryPlugin

DEFINES += override="" \
           IMAGEDANIMATIONPLUGIN_EXPORT

LIBS += -L$$PWD/../../bin -lCorePlugin

DESTDIR = $$PWD/../../bin
