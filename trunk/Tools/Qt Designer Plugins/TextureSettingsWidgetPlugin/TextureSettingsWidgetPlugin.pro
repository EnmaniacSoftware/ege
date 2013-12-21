include(TextureSettingsWidgetPlugin.pri)

CONFIG      += plugin

QT          += widgets designer

INCLUDEPATH += Interface

TEMPLATE    = lib

target.path += $$[QT_INSTALL_PLUGINS]/designer \
#target.path += $$[QT_INSTALL_PREFIX]/../../Tools/QtCreator/bin/plugins/designer

INSTALLS += target
