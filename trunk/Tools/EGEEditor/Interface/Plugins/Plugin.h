#ifndef PLUGIN_INTERFACE_H
#define PLUGIN_INTERFACE_H

#include <QObject>
#include <qplugin.h>

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
Q_DECLARE_INTERFACE(IPlugin, "com.littlebeestudios.plugin.iplugin/1.0")
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define EGE_EXPORT_PLUGIN(pluginClass) \
            Q_EXTERN_C Q_DECL_EXPORT QT_PREPEND_NAMESPACE(QObject)* ege_plugin_instance() Q_PLUGIN_INSTANCE(pluginClass)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef QT_PREPEND_NAMESPACE(QObject)*  (*PFPLUGININSTANCE) ();
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // PLUGIN_INTERFACE_H
