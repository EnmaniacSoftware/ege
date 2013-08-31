CONFIG      += plugin

QT          += widgets

TEMPLATE    = lib

HEADERS     = IResourceLibrary.h \
              IResourceLibraryDataModel.h \
              ResourceItem.h \
              ResourceItemContainer.h \
              ResourceItemFactory.h \
              ResourceItemImage.h \
              ResourceLibraryDataModel.h \
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

FORMS       += ResourceLibraryWindow.ui

INCLUDEPATH += ../../EGEEditor/src \
               ../../EditorPlugins/CorePlugin

DEFINES += override="" \
           RESOUCELIBRARYPLUGIN_EXPORT=1

LIBS += -L$$PWD/../../bin -lCorePlugin

DESTDIR = $$PWD/../../bin
