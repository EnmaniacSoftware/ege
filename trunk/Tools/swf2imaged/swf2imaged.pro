CONFIG += qt
QT += xml

SOURCES = main.cpp \
    SwfParser.cpp \
    SwfFile.cpp \
    SwfHeader.cpp \
    SwfDataStream.cpp \
    SwfFileAttributeTag.cpp \
    SwfTag.cpp

HEADERS = \ 
    SwfParser.h \
    SwfFile.h \
    SwfFileAttributeTag.h \
    SwfHeader.h \
    SwfDataStream.h \
    SwfTag.h \
    SwfTypes.h
