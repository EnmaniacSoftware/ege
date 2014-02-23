HEADERS += Interface/CorePlugin_global.h \
           Interface/ObjectPool.h \
           Interface/FileSystemUtils.h \
           Interface/Settings.h \
           Interface/Projects/Project.h \
           Interface/Projects/ProjectFactory.h \
           Implementation/CorePlugin.h \
    Interface/UuidList.h \
    Interface/Attachable.h

SOURCES += Implementation/CorePlugin.cpp \
           Implementation/ObjectPool.cpp \
           Implementation/FileSystemUtils.cpp \
           Implementation/Settings.cpp \
           Implementation/Projects/Project.cpp \
           Implementation/Projects/ProjectFactory.cpp

RESOURCES += Resources/core.qrc
