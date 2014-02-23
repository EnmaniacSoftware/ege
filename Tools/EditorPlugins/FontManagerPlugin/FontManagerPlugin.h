#ifndef FONTMANAGERPLUGIN_H
#define FONTMANAGERPLUGIN_H

#include <Plugins/Plugin.h>
#include "fontmanagerplugin_global.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class FONTMANAGERPLUGIN_API FontManagerPlugin : public QObject, public IPlugin
{
  Q_OBJECT
  Q_INTERFACES(IPlugin)

  public:

    explicit FontManagerPlugin(QObject* parent = NULL);
    ~FontManagerPlugin();

  private:

    /* IPlugin override. Initialized plugin. */
    bool initialize() override;
    /* IPlugin override. Deinitializes plugin. */
    void deinitialize() override;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // FONTMANAGERPLUGIN_H
