CONFIG      += plugin

TARGET      = $$qtLibraryTarget($$TARGET)
TEMPLATE    = lib

HEADERS     = JuiceJumpersProjectPlugin.h \
              ProjectJuiceJumpers.h

SOURCES     = JuiceJumpersProjectPlugin.cpp \
              ProjectJuiceJumpers.cpp

INCLUDEPATH += ../../EGEEditor/src \
               ../../EditorPlugins/CorePlugin \
               ../../EditorPlugins/ResourceLibraryPlugin

DEFINES += override="" JUICEJUMPERSPROJECTPLUGIN_EXPORT

INSTALLS += target
