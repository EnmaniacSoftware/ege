#ifndef EDITOR_PLUGINS_PLUGIN_H
#define EDITOR_PLUGINS_PLUGIN_H

/*! Interface for editor plugin. */

#include <QObject>
#include <qplugin.h>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class IPlugin
{
  public:

    virtual ~IPlugin() {}

    /*! Initialized plugin. */
    virtual bool initialize() = 0;
    /*! Deinitializes plugin. */
    virtual void deinitialize() = 0;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Q_DECLARE_INTERFACE(IPlugin, "com.littlebeestudios.plugin.iplugin/1.0")
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define EGE_EXPORT_PLUGIN(pluginClass) \
            Q_EXTERN_C Q_DECL_EXPORT QT_PREPEND_NAMESPACE(QObject)* ege_plugin_instance() Q_PLUGIN_INSTANCE(pluginClass)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef QT_PREPEND_NAMESPACE(QObject)*  (*PFPLUGININSTANCE) ();
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EDITOR_PLUGINS_PLUGIN_H
