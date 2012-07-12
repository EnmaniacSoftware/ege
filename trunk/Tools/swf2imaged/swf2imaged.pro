CONFIG += qt
QT += xml

SOURCES = main.cpp \
    ResourceManager.cpp \
    SwfDataStream.cpp \
    SwfDefineBitsJpeg3Tag.cpp \
    SwfDefineBitsLossless2Tag.cpp \
    SwfDefineBitsLosslessTag.cpp \
    SwfDefineShape2Tag.cpp \
    SwfDefineShapeTag.cpp \
    SwfEndTag.cpp \
    SwfFile.cpp \
    SwfFileAttributeTag.cpp \
    SwfHeader.cpp \
    SwfParser.cpp \
    SwfDataStream.cpp \
    SwfFileAttributeTag.cpp \
    SwfPlaceObject2Tag.cpp \
    SwfRemoveObject2Tag.cpp \
    SwfRemoveObjectTag.cpp \
    SwfSetBackgroundColorTag.cpp \
    SwfShowFrameTag.cpp \
    SwfTag.cpp

HEADERS = \
    ResourceManager.h \
    SwfDataStream.h \
    SwfDefineBitsJpeg3Tag.h \
    SwfDefineBitsLossless2Tag.h \
    SwfDefineBitsLosslessTag.h \
    SwfDefineShape2Tag.h \
    SwfDefineShapeTag.h \
    SwfEndTag.h \
    SwfFile.h \
    SwfFileAttributeTag.h \
    SwfHeader.h \
    SwfParser.h \
    SwfPlaceObject2Tag.h \
    SwfRemoveObject2Tag.h \
    SwfRemoveObjectTag.h \
    SwfSetBackgroundColorTag.h \
    SwfShowFrameTag.h \
    SwfTag.h \
    SwfTypes.h
