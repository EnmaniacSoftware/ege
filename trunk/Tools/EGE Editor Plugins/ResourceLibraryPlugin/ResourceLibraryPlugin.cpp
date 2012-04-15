#include "ResourceLibraryPlugin.h"
#include "ResourceLibraryWindow.h"
#include "ResourceItemFactory.h"
#include <QtPlugin>
#include <QDebug>
#include <Core.h>
#include <MainWindow.h>
#include <ObjectPool.h>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResouceLibraryPlugin::ResouceLibraryPlugin(QObject* parent) : QObject(parent),
                                                              m_window(NULL),
                                                              m_resourceItemFactory(NULL)
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
  Core* core = Core::Instance();
  MainWindow* mainWindow = core->mainWindow();

  m_window = new ResourceLibraryWindow(mainWindow);
  m_resourceItemFactory = new ResourceItemFactory();

  if ((NULL != m_window) && (NULL != m_resourceItemFactory))
  {
    return ObjectPool::Instance()->addObject(m_window) && ObjectPool::Instance()->addObject(m_resourceItemFactory);
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IPlugin override. Deinitializes plugin. */
void ResouceLibraryPlugin::deinitialize()
{
  if (m_resourceItemFactory)
  {
    ObjectPool::Instance()->removeObject(m_resourceItemFactory);

    delete m_resourceItemFactory;
    m_resourceItemFactory = NULL;
  }

  if (m_window)
  {
    ObjectPool::Instance()->removeObject(m_window);

    delete m_window;
    m_window = NULL;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_EXPORT_PLUGIN(ResouceLibraryPlugin)
