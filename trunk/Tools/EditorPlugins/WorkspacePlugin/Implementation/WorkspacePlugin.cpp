#include "WorkspacePlugin.h"
#include "MainWindow.h"
#include "Configuration.h"
#include "PropertiesWindow.h"
#include <QStatusBar>
#include <ObjectPool.h>
#include <Projects/ProjectFactory.h>
#include <QtPlugin>
#include <QDebug>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
WorkspacePlugin::WorkspacePlugin(QObject* parent) : QObject(parent),
                                                    m_mainWindow(NULL),
                                                    m_configuration(NULL),
                                                    m_propertiesWindow(NULL)
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
  m_mainWindow        = new MainWindow();
  m_configuration     = new Configuration();
  m_propertiesWindow  = new PropertiesWindow(m_mainWindow);

  // add to pool
  bool result = ObjectPool::Instance()->addObject(m_mainWindow) && ObjectPool::Instance()->addObject(m_configuration) &&
                ObjectPool::Instance()->addObject(m_propertiesWindow);
  if (result)
  {
    // get project factory
    ProjectFactory* projectFactory = ObjectPool::Instance()->getObject<ProjectFactory>();
    Q_ASSERT(NULL != projectFactory);

    result = (NULL != projectFactory);
    if (result)
    {
      // initial placement
      m_mainWindow->addDockWidget(Qt::RightDockWidgetArea, m_propertiesWindow);

      // connect for notifications
      connect(m_mainWindow, SIGNAL(saveData(QXmlStreamWriter&)), projectFactory, SLOT(onSaveData(QXmlStreamWriter&)));
      connect(m_mainWindow, SIGNAL(loadData(QXmlStreamReader&)), projectFactory, SLOT(onLoadData(QXmlStreamReader&)));
      connect(m_mainWindow, SIGNAL(saveData(QXmlStreamWriter&)), m_configuration, SLOT(onSaveData(QXmlStreamWriter&)));
      connect(m_mainWindow, SIGNAL(loadData(QXmlStreamReader&)), m_configuration, SLOT(onLoadData(QXmlStreamReader&)));

      // add configuration to main window
      m_mainWindow->statusBar()->addPermanentWidget(m_configuration);

      // show main window
      m_mainWindow->show();
    }
  }

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
