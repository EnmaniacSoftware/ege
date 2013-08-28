CONFIG      += plugin

TARGET      = $$qtLibraryTarget($$TARGET)
TEMPLATE    = lib

HEADERS     = AddConfigurationWindow.h \
              Configuration.h \
              CoreConstants.h \
              CorePlugin.h \
              FSUtils.h \
              MainWindow.h \
              NewProjectWindow.h \
              ObjectPool.h \
              Project.h \
              ProjectFactory.h \
              Serializer.h

SOURCES     = AddConfigurationWindow.cpp \
              Configuration.cpp \
              CorePlugin.cpp \
              FSUtils.cpp \
              MainWindow.cpp \
              NewProjectWindow.cpp \
              ObjectPool.cpp \
              Project.cpp \
              ProjectFactory.cpp

FORMS       = addconfiguration.ui \
              configuration.ui \
              mainwindow.ui \
              newproject.ui

INCLUDEPATH += ../../EGEEditor/src

DEFINES += override=""

INSTALLS += target
