#include "ResourceLibraryPlugin.h"
#include "ResourceLibraryWindow.h"
#include "ResourceItemFactory.h"
#include <MainWindow.h>
#include <ObjectPool.h>
#include <QtPlugin>
#include <QDebug>
#include <QResource>
#include <QMenu>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResouceLibraryPlugin::ResouceLibraryPlugin(QObject* parent) : QObject(parent),
                                                              m_window(NULL),
                                                              m_resourceItemFactory(NULL),
                                                              m_viewAction(NULL)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResouceLibraryPlugin::~ResouceLibraryPlugin()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ResouceLibraryPlugin::initialize()
{
  // register resources
  Q_INIT_RESOURCE(resources);

  MainWindow* mainWindow = ObjectPool::Instance()->getObject<MainWindow>();
  Q_ASSERT(NULL != mainWindow);

  if (NULL == mainWindow)
  {
    // error!
    return false;
  }

  // create objects
  m_window              = new ResourceLibraryWindow(mainWindow);
  m_resourceItemFactory = new ResourceItemFactory();

  // add to pool
  bool result = ObjectPool::Instance()->addObject(m_window) && ObjectPool::Instance()->addObject(m_resourceItemFactory);
  if (result)
  {
    // initial placement
    mainWindow->addDockWidget(Qt::LeftDockWidgetArea, m_window);
  }

  // create menu entries
  QMenu* menu = mainWindow->menu(MainWindow::EView);
  Q_ASSERT(menu);

  // create actions
  m_viewAction = new QAction(tr("Resource library"), menu);

  // setup actions
  m_viewAction->setShortcut(QKeySequence("Ctrl+Shift+R"));
  connect(m_viewAction, SIGNAL(triggered()), m_window, SLOT(show()));

  // add actions to menu
  menu->addAction(m_viewAction);

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResouceLibraryPlugin::deinitialize()
{
  MainWindow* mainWindow = ObjectPool::Instance()->getObject<MainWindow>();
  Q_ASSERT(NULL != mainWindow);

  // delete menu actions
  QMenu* menu = mainWindow->menu(MainWindow::EView);
  Q_ASSERT(menu);

  if (NULL != m_viewAction)
  {
    // remove from menu
    menu->removeAction(m_viewAction);

    // delete
    delete m_viewAction;
    m_viewAction = NULL;
  }

  if (NULL != m_resourceItemFactory)
  {
    // remove from pool
    ObjectPool::Instance()->removeObject(m_resourceItemFactory);

    // delete
    delete m_resourceItemFactory;
    m_resourceItemFactory = NULL;
  }

  if (NULL != m_window)
  {
    // remove from pool
    ObjectPool::Instance()->removeObject(m_window);

    // delete
    delete m_window;
    m_window = NULL;
  }

  // clean up resources
  Q_CLEANUP_RESOURCE(resources);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_EXPORT_PLUGIN(ResouceLibraryPlugin)
