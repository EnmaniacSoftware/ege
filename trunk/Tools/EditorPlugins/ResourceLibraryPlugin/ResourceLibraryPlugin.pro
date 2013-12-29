include(ResourceLibraryPlugin.pri)
include(../PluginExtras.pri)

QT += widgets

INCLUDEPATH += Interface \
            ../CorePlugin/Interface \
            ../WorkspacePlugin/Interface \
            ../../QtDesignerPlugins/TextureSettingsWidgetPlugin/Interface

DEFINES += RESOUCELIBRARYPLUGIN_EXPORT=1

LIBS += -lWorkspacePlugin -lCorePlugin -lTextureSettingsWidgetPlugin

target.path = ../../EGEEditor/Plugins
INSTALLS += target
