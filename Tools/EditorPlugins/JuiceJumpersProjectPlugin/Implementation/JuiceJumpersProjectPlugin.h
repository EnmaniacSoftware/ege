#ifndef PLUGIN_JUICEJUMPERSPROJECTPLUGIN_H
#define PLUGIN_JUICEJUMPERSPROJECTPLUGIN_H

#include <Plugins/Plugin.h>
#include "JuiceJumpersProjectPlugin_global.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class JUICEJUMPERSPROJECTPLUGIN_API JuiceJumpersProjectPlugin : public QObject, public IPlugin
{
  Q_OBJECT
  Q_INTERFACES(IPlugin)

  public:
  
    explicit JuiceJumpersProjectPlugin(QObject* parent = NULL);
    ~JuiceJumpersProjectPlugin();

  private:

    /*! @see IPlugin::initialize. */
    bool initialize() override;
    /*! @see IPlugin::deinitialize. */
    void deinitialize() override;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // PLUGIN_JUICEJUMPERSPROJECTPLUGIN_H
