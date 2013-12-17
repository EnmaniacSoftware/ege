HEADERS = Implementation/ResourceLibraryPlugin.h \
          Implementation/ResourceLibraryDataModel.h \
          Interface/ResourceLibraryItemDelegate.h \
          Interface/ResourceLibraryWindow.h \
          Interface/ResourceItem.h \
          Interface/ResourceItemContainer.h \
          Interface/ResourceItemImage.h \
          Interface/ResourceItemFactory.h \
          Interface/ResouceLibraryPlugin_global.h \
    Interface/ResourceLibrary.h

SOURCES = Implementation/ResourceLibraryPlugin.cpp \
          Implementation/ResourceLibraryWindow.cpp \
          Implementation/ResourceItemFactory.cpp \
          Implementation/ResourceItem.cpp \
          Implementation/ResourceItemContainer.cpp \
          Implementation/ResourceItemImage.cpp \
          Implementation/ResourceLibraryDataModel.cpp \
          Implementation/ResourceLibraryItemDelegate.cpp \
    Implementation/ResourceLibrary.cpp

FORMS += Forms/ResourceLibraryWindow.ui

RESOURCES += Resources/resources.qrc
