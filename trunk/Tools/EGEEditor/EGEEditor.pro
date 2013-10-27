include(EGEEditor.pri)

TEMPLATE = app

QT += core widgets

INCLUDEPATH	+= Interface

# plugins installation
win32:plugins.files += ../bin/*.dll
linux:plugins.files += ../bin/*.so
plugins.path = Plugins

INSTALLS += plugins
