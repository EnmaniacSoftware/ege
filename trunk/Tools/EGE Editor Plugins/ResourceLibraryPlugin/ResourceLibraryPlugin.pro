CONFIG      += plugin

TARGET      = $$qtLibraryTarget($$TARGET)
TEMPLATE    = lib

HEADERS     = IResourceLibrary.h \
              IResourceLibraryDataModel.h \
              ResourceItem.h \
              ResourceItemContainer.h \
              ResourceItemFactory.h \
              ResourceItemImage.h \
              ResourceLibararyDataModel.h \
              ResourceLibraryItemDelegate.h \
              ResourceLibraryPlugin.h \
              ResourceLibraryWindow.h

SOURCES     = ResourceItem.cpp \
              ResourceItemContainer.cpp \
              ResourceItemFactory.cpp \
              ResourceItemImage.cpp \
              ResourceLibraryDataModel.cpp \
              ResourceLibraryItemDelegate.cpp \
              ResourceLibraryPlugin.cpp \
              ResourceLibraryWindow.cpp

FORMS       = ResourceLibraryWindow.ui

INCLUDEPATH += ../../EGEEditor/src \
              "../../EGE Editor Plugins/CorePlugin"

DEFINES += override=""

INSTALLS += target
