HEADERS = Implementation/ResourceLibraryPlugin.h \
          Implementation/ResourceLibraryDataModel.h \
          Interface/ResourceLibraryItemDelegate.h \
          Interface/ResourceLibraryWindow.h \
          Interface/ResourceItem.h \
          Interface/ResourceItemTexture.h \
          Interface/ResourceItemFactory.h \
          Interface/ResouceLibraryPlugin_global.h \
          Interface/ResourceLibrary.h \
          Interface/ResourceItemGroup.h \
          Implementation/ResourceLibraryWindowResourceInserter.h \
    Implementation/ResourceLibraryDataModelProxy.h

SOURCES = Implementation/ResourceLibraryPlugin.cpp \
          Implementation/ResourceLibraryWindow.cpp \
          Implementation/ResourceItemFactory.cpp \
          Implementation/ResourceItem.cpp \
          Implementation/ResourceItemTexture.cpp \
          Implementation/ResourceLibraryDataModel.cpp \
          Implementation/ResourceLibraryItemDelegate.cpp \
          Implementation/ResourceLibrary.cpp \
          Implementation/ResourceItemGroup.cpp \
          Implementation/ResourceLibraryWindowResourceInserter.cpp \
    Implementation/ResourceLibraryDataModelProxy.cpp

FORMS += Forms/ResourceLibraryWindow.ui

RESOURCES += Resources/resources.qrc
