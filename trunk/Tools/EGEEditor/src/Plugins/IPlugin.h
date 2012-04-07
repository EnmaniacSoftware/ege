#ifndef PLUGIN_INTERFACE_H
#define PLUGIN_INTERFACE_H

#include <QObject>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class IPlugin
{
  public:

    /*! Initialized plugin. */
    virtual bool initialize() = 0;
    /*! Deinitializes plugin. */
    virtual void deinitialize() = 0;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Q_DECLARE_INTERFACE(IPlugin, "com.littlebeestudios.plugin.iplugin/1.0");
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define EGE_EXPORT_PLUGIN(pluginName, pluginClass, pluginDependencies) \
            Q_EXTERN_C Q_DECL_EXPORT const char * Q_STANDARD_CALL ege_plugin_dependencies() { return pluginDependencies; } \
            Q_EXTERN_C Q_DECL_EXPORT const char * Q_STANDARD_CALL ege_plugin_name() { return pluginName; } \
            Q_EXTERN_C Q_DECL_EXPORT QT_PREPEND_NAMESPACE(QObject) * Q_STANDARD_CALL ege_plugin_instance() Q_PLUGIN_INSTANCE(pluginClass)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef const char*                     (*PFPLUGINDEPENDENCIES) ();
typedef const char*                     (*PFPLUGINNAME) ();
typedef QT_PREPEND_NAMESPACE(QObject)*  (*PFPLUGININSTANCE) ();
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // PLUGIN_INTERFACE_H