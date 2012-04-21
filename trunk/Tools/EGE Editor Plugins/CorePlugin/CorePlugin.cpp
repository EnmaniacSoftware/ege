#include "coreplugin.h"
#include "ObjectPool.h"
#include "MainWindow.h"
#include "ProjectFactory.h"
#include "Configuration.h"
#include <QtPlugin>
#include <QResource>
#include <QDebug>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
CorePlugin::CorePlugin(QObject* parent) : QObject(parent),
                                          m_mainWindow(NULL),
                                          m_projectFactory(NULL),
                                          m_configuration(NULL)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
CorePlugin::~CorePlugin()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IPlugin override. Initialized plugin. */
bool CorePlugin::initialize()
{
  // register resources
  Q_INIT_RESOURCE(core);

  m_mainWindow      = new MainWindow();
  m_projectFactory  = new ProjectFactory();
  m_configuration   = new Configuration();

  // initialize main window
  if (!m_mainWindow->initialize())
  {
    // error!
    return false;
  }

  // add to pool
  if (!ObjectPool::Instance()->addObject(m_mainWindow) ||
      !ObjectPool::Instance()->addObject(m_configuration) ||
      !ObjectPool::Instance()->addObject(m_projectFactory))
  {
    // error!
    return false;
  }

  m_mainWindow->show();

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IPlugin override. Deinitializes plugin. */
void CorePlugin::deinitialize()
{
  if (NULL != m_projectFactory)
  {
    delete m_projectFactory;
    m_projectFactory = NULL;
  }

  if (NULL != m_mainWindow)
  {
    delete m_mainWindow;
    m_mainWindow = NULL;
  }

  Q_CLEANUP_RESOURCE(core);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_EXPORT_PLUGIN(CorePlugin)
