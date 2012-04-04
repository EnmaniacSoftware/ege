#ifndef COREPLUGIN_H
#define COREPLUGIN_H

#include <Plugins/IPlugin.h>
#include "coreplugin_global.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class COREPLUGIN_API CorePlugin : public QObject, public IPlugin
{
  Q_OBJECT
  Q_INTERFACES(IPlugin)

  public:
  
    explicit CorePlugin(QObject* parent = NULL);
    ~CorePlugin();

    /* IPlugin override. Initialized plugin. */
    bool initialize() override;
    /* IPlugin override. Deinitializes plugin. */
    void deinitialize() override;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // COREPLUGIN_H
