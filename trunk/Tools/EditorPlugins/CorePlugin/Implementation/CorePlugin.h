#ifndef CORE_COREPLUGIN_H
#define CORE_COREPLUGIN_H

#include <Plugins/Plugin.h>
#include "CorePlugin_global.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ProjectFactory;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class COREPLUGIN_API CorePlugin : public QObject, public IPlugin
{
  Q_OBJECT
  Q_INTERFACES(IPlugin)

  public:
  
    explicit CorePlugin(QObject* parent = NULL);
    ~CorePlugin();

    /*! @see IPlugin::initialize. */
    bool initialize() override;
    /*! @see IPlugin::deinitialize. */
    void deinitialize() override;

  private:

    /*! Project factory. */
    ProjectFactory* m_projectFactory;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // CORE_COREPLUGIN_H
