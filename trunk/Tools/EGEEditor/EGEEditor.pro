TEMPLATE = app

QT += core gui widgets

HEADERS = Implementation/Application.h \
          Interface/Config.h \
          Interface/FSUtils.h \
          Interface/MainWindow.h \
          Interface/ObjectPool.h \
          Interface/Serializer.h \
          Interface/Plugins/PluginsManager.h \
          Interface/Plugins/Plugin.h

HEADERS += Interface/Projects/ProjectFactory.h \
           Interface/Projects/Project.h \
           Implementation/Projects/NewProjectWindow.h

SOURCES	= Implementation/main.cpp \
          Implementation/Application.cpp \
          Implementation/Config.cpp \
          Implementation/FSUtils.cpp \
          Implementation/MainWindow.cpp \
          Implementation/ObjectPool.cpp \
          Implementation/Plugins/PluginsManager.cpp

SOURCES += Implementation/Projects/NewProjectWindow.cpp \
           Implementation/Projects/ProjectFactory.cpp \
           Implementation/Projects/Project.cpp

FORMS += Forms/mainwindow.ui \
         Forms/newproject.ui

INCLUDEPATH	+= Interface

Debug:UI_DIR   = debug/.ui
Release:UI_DIR = release/.ui

#INSTALLS += target
