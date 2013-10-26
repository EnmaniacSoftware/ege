#include "CorePlugin.h"
#include "ObjectPool.h"
#include "Projects/ProjectFactory.h"
#include <QtPlugin>
#include <QResource>
#include <QDebug>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
CorePlugin::CorePlugin(QObject* parent) : QObject(parent)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
CorePlugin::~CorePlugin()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CorePlugin::initialize()
{
  // register resources
  Q_INIT_RESOURCE(core);

//  m_mainWindow      = new MainWindow();
  m_projectFactory  = new ProjectFactory();
 // m_configuration   = new Configuration();

  // initialize main window
//  if (!m_mainWindow->initialize())
//  {
//    // error!
//    return false;
//  }

  // add to pool
  if (/*!ObjectPool::Instance()->addObject(m_mainWindow) ||*/
//      !ObjectPool::Instance()->addObject(m_configuration) ||
      !ObjectPool::Instance()->addObject(m_projectFactory))
  {
    // error!
    return false;
  }

//  m_mainWindow->show();

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void CorePlugin::deinitialize()
{
  if (NULL != m_projectFactory)
  {
    delete m_projectFactory;
    m_projectFactory = NULL;
  }

  Q_CLEANUP_RESOURCE(core);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_EXPORT_PLUGIN(CorePlugin)
