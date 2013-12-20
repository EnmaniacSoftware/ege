#ifndef RESOURCELIBRARY_RESOUCELIBRARYPLUGIN_H
#define RESOURCELIBRARY_RESOUCELIBRARYPLUGIN_H

#include <Plugins/Plugin.h>
#include "resoucelibraryplugin_global.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceLibraryWindow;
class ResourceItemFactory;
class ResourceLibrary;
class ResourceLibraryWindowResourceInserter;
class QAction;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class RESOUCELIBRARYPLUGIN_API ResouceLibraryPlugin : public QObject, public IPlugin
{
  Q_OBJECT
  Q_INTERFACES(IPlugin)

  public:
  
    explicit ResouceLibraryPlugin(QObject* parent = NULL);
    ~ResouceLibraryPlugin();

  private:

    /*! @see IPlugin::initialize. */
    bool initialize() override;
    /*! @see IPlugin::deinitialize. */
    void deinitialize() override;

  private:

    /*! Module window. */
    ResourceLibraryWindow* m_window;
    /*! Resource library. */
    ResourceLibrary* m_library;
    /*! Resource item factory. */
    ResourceItemFactory* m_resourceItemFactory;
    /*! Helper class for adding resources by means of ResourceLibraryWindow's context menu. */
    ResourceLibraryWindowResourceInserter* m_windowResourceInserter;
    /*! View menu action. */
    QAction* m_viewAction;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // RESOURCELIBRARY_RESOUCELIBRARYPLUGIN_H
