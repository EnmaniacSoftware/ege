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

#endif // PLUGIN_INTERFACE_H