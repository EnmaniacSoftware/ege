HEADERS +=  Interface/WorkspacePlugin_global.h \
            Interface/MainWindow.h \
            Interface/Configuration.h \
            Implementation/WorkspacePlugin.h \
            Implementation/NewProjectWindow.h \
            Implementation/AddConfigurationWindow.h \
            Interface/PropertiesWindow.h \
            Interface/PropertyObject.h \
            Interface/PropertyDefinition.h \
            Implementation/FilePathPropertyManager.h \
            Implementation/FilePathEditFactory.h \
            Implementation/FilePathEditWidget.h \
    Implementation/ChangeConfigurationNameWindow.h \
    Implementation/ConfigurationNameValidator.h \
    Interface/PropertyValueHelper.h

SOURCES +=  Implementation/WorkspacePlugin.cpp \
            Implementation/MainWindow.cpp \
            Implementation/NewProjectWindow.cpp \
            Implementation/AddConfigurationWindow.cpp \
            Implementation/Configuration.cpp \
            Implementation/PropertiesWindow.cpp \
            Implementation/PropertyObject.cpp \
            Implementation/PropertyDefinition.cpp \
            Implementation/FilePathPropertyManager.cpp \
            Implementation/FilePathEditFactory.cpp \
            Implementation/FilePathEditWidget.cpp \
    Implementation/ChangeConfigurationNameWindow.cpp \
    Implementation/ConfigurationNameValidator.cpp \
    Implementation/PropertyValueHelper.cpp

FORMS += Forms/mainwindow.ui \
         Forms/newproject.ui \
         Forms/addconfiguration.ui \
         Forms/configuration.ui \
         Forms/PropertiesWindow.ui \
         Forms/FilePathEditWidget.ui \
    Forms/ChangeNameDialog.ui
