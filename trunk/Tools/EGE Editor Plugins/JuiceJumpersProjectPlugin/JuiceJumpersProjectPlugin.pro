CONFIG      += plugin

TARGET      = $$qtLibraryTarget($$TARGET)
TEMPLATE    = lib

HEADERS     = JuiceJumpersProjectPlugin.h \
              ProjectJuiceJumpers.h

SOURCES     = JuiceJumpersProjectPlugin.cpp \
              ProjectJuiceJumpers.cpp

INCLUDEPATH += ../../EGEEditor/src \
               "../../EGE Editor Plugins/CorePlugin" \
               "../../EGE Editor Plugins/ResourceLibraryPlugin"

DEFINES += override=""

INSTALLS += target
