HEADERS += Interface/WorkspacePlugin_global.h \
           Interface/MainWindow.h \
           Interface/Configuration.h \
           Implementation/WorkspacePlugin.h \
           Implementation/NewProjectWindow.h \
           Implementation/AddConfigurationWindow.h \
    Interface/PropertiesWindow.h

SOURCES += Implementation/WorkspacePlugin.cpp \
           Implementation/MainWindow.cpp \
           Implementation/NewProjectWindow.cpp \
           Implementation/AddConfigurationWindow.cpp \
           Implementation/Configuration.cpp \
    Implementation/PropertiesWindow.cpp

FORMS += Forms/mainwindow.ui \
         Forms/newproject.ui \
         Forms/addconfiguration.ui \
         Forms/configuration.ui \
    Forms/PropertiesWindow.ui
