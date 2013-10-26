#include "WorkspacePlugin.h"
#include "MainWindow.h"
#include <ObjectPool.h>
#include <QtPlugin>
#include <QDebug>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
WorkspacePlugin::WorkspacePlugin(QObject* parent) : QObject(parent),
                                                    m_mainWindow(NULL)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
WorkspacePlugin::~WorkspacePlugin()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool WorkspacePlugin::initialize()
{
  // create main window
  m_mainWindow = new MainWindow();

  // initialize and add to pool
  bool result = m_mainWindow->initialize();

  // add to pool
  if (result)
  {
    result = ObjectPool::Instance()->addObject(m_mainWindow);
  }

  // show main window
  m_mainWindow->show();

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void WorkspacePlugin::deinitialize()
{
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
