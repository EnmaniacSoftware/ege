#include "WorkspacePlugin.h"
#include "MainWindow.h"
#include "Configuration.h"
#include <ObjectPool.h>
#include <QtPlugin>
#include <QDebug>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
WorkspacePlugin::WorkspacePlugin(QObject* parent) : QObject(parent),
                                                    m_mainWindow(NULL),
                                                    m_configuration(NULL)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
WorkspacePlugin::~WorkspacePlugin()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool WorkspacePlugin::initialize()
{
  // create objects
  m_mainWindow = new MainWindow();
  m_configuration = new Configuration();

  // add to pool
  bool result = ObjectPool::Instance()->addObject(m_mainWindow) && ObjectPool::Instance()->addObject(m_configuration);

  // add configuration to main window
  m_mainWindow->statusBar()->addPermanentWidget(m_configuration);

  // show main window
  m_mainWindow->show();

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void WorkspacePlugin::deinitialize()
{
  if (NULL != m_configuration)
  {
    // remove from pool
    ObjectPool::Instance()->removeObject(m_configuration);

    // deallocate
    delete m_configuration;
    m_configuration = NULL;
  }

  if (NULL != m_mainWindow)
  {
    // remove from pool
    ObjectPool::Instance()->removeObject(m_mainWindow);

    // deallocate
    delete m_mainWindow;
    m_mainWindow = NULL;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_EXPORT_PLUGIN(WorkspacePlugin)
