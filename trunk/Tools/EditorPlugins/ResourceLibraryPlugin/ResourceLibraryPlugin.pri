HEADERS = Implementation/ResourceLibraryPlugin.h \
          Implementation/ResourceLibraryDataModel.h \
          Interface/ResourceLibraryItemDelegate.h \
          Interface/ResourceLibraryWindow.h \
          Interface/ResourceItem.h \
          Interface/ResourceItemFactory.h \
          Interface/ResouceLibraryPlugin_global.h \
          Interface/ResourceLibrary.h \
          Interface/ResourceItemGroup.h \
          Implementation/ResourceLibraryWindowResourceInserter.h \
          Implementation/ResourceLibraryDataModelProxy.h \
    Implementation/ImageFormats.h \
    Implementation/TextureAddressingModes.h \
    Implementation/ResourceItemTexture.h \
    Implementation/ResourceItemTextureAtlas.h

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
          Implementation/ResourceLibraryDataModelProxy.cpp \
          Implementation/ResourceItemTextureAtlas.cpp \
    Implementation/ImageFormats.cpp \
    Implementation/TextureAddressingModes.cpp

FORMS += Forms/ResourceLibraryWindow.ui \
         Forms/TexturePropertiesWindow.ui

RESOURCES += Resources/resources.qrc
