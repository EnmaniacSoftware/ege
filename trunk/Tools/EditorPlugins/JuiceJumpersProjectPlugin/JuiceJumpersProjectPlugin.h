#ifndef JUICEJUMPERSPROJECTPLUGIN_H
#define JUICEJUMPERSPROJECTPLUGIN_H

#include <Plugins/IPlugin.h>
#include "juicejumpersprojectplugin_global.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class JUICEJUMPERSPROJECTPLUGIN_API JuiceJumpersProjectPlugin : public QObject, public IPlugin
{
  Q_OBJECT
  Q_INTERFACES(IPlugin)

  public:
  
    explicit JuiceJumpersProjectPlugin(QObject* parent = NULL);
    ~JuiceJumpersProjectPlugin();

  private:

    /* IPlugin override. Initialized plugin. */
    bool initialize() override;
    /* IPlugin override. Deinitializes plugin. */
    void deinitialize() override;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // JUICEJUMPERSPROJECTPLUGIN_H
