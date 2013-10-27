HEADERS += Interface/CorePlugin_global.h \
           Interface/ObjectPool.h \
           Interface/FileSystemUtils.h \
           Interface/Projects/Project.h \
           Interface/Projects/ProjectFactory.h \
           Implementation/CorePlugin.h

SOURCES += Implementation/CorePlugin.cpp \
           Implementation/ObjectPool.cpp \
           Implementation/FileSystemUtils.cpp \
           Implementation/Projects/Project.cpp \
           Implementation/Projects/ProjectFactory.cpp

RESOURCES += Resources/core.qrc
