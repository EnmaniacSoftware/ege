include(ResourceLibraryPlugin.pri)
include(../PluginExtras.pri)

CONFIG += TextureSettingsWidgetPlugin

QT += widgets

INCLUDEPATH += Interface \
            ../CorePlugin/Interface \
            ../WorkspacePlugin/Interface \
            ../../QtDesignerPlugins/TextureSettingsWidgetPlugin/Interface

DEFINES += RESOUCELIBRARYPLUGIN_EXPORT=1

LIBS += -lWorkspacePlugin -lCorePlugin

target.path = ../../EGEEditor/Plugins
INSTALLS += target
