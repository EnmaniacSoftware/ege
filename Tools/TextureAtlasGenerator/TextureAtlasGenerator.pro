TEMPLATE = app

QT += xml core widgets
QT -= gui

CONFIG += console

SOURCES = main.cpp \
          TextureAtlasGenerator.cpp \
          AtlasGroup.cpp \
          AtlasGroupEntry.cpp

HEADERS = TextureAtlasGenerator.h \
          AtlasGroup.h \
          AtlasGroupEntry.h \
          AtlasNode.h
