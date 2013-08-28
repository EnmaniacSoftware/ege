#ifndef RESOUCELIBRARYPLUGIN_H
#define RESOUCELIBRARYPLUGIN_H

#include <Plugins/IPlugin.h>
#include "resoucelibraryplugin_global.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceLibraryWindow;
class ResourceItemFactory;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class RESOUCELIBRARYPLUGIN_API ResouceLibraryPlugin : public QObject, public IPlugin
{
  Q_OBJECT
  Q_INTERFACES(IPlugin)

  public:
  
    explicit ResouceLibraryPlugin(QObject* parent = NULL);
    ~ResouceLibraryPlugin();

  private:

    /* IPlugin override. Initialized plugin. */
    bool initialize() override;
    /* IPlugin override. Deinitializes plugin. */
    void deinitialize() override;

  private:

    /*! Module window. */
    ResourceLibraryWindow* m_window;
    /*! Resource item factory. */
    ResourceItemFactory* m_resourceItemFactory;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // RESOUCELIBRARYPLUGIN_H
