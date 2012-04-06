#include "resoucelibraryplugin.h"
#include <QtPlugin>
#include <QDebug>
#include <Core.h>
#include <MainWindow.h>
#include "ResourceLibraryWindow.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResouceLibraryPlugin::ResouceLibraryPlugin(QObject* parent) : QObject(parent),
                                                              m_resourceLibraryWindow(NULL)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResouceLibraryPlugin::~ResouceLibraryPlugin()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IPlugin override. Initialized plugin. */
bool ResouceLibraryPlugin::initialize()
{
  Core* core = Core::instance();
  MainWindow* mainWindow = core->mainWindow();

  m_resourceLibraryWindow = new ResourceLibraryWindow(mainWindow);

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IPlugin override. Deinitializes plugin. */
void ResouceLibraryPlugin::deinitialize()
{
  if (m_resourceLibraryWindow)
  {
    delete m_resourceLibraryWindow;
    m_resourceLibraryWindow = NULL;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_EXPORT_PLUGIN("resourcelibrary", ResouceLibraryPlugin, "core")
