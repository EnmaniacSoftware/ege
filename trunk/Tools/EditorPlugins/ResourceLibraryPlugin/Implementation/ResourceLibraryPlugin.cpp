#include "ResourceLibraryPlugin.h"
#include "ResourceLibrary.h"
#include "ResourceLibraryWindow.h"
#include "ResourceItemFactory.h"
#include "ResourceLibraryWindowGroupAdder.h"
#include <MainWindow.h>
#include <ObjectPool.h>
#include <QtPlugin>
#include <QDebug>
#include <QResource>
#include <QMenu>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResouceLibraryPlugin::ResouceLibraryPlugin(QObject* parent) : QObject(parent),
                                                              m_window(NULL),
                                                              m_library(NULL),
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
  m_library             = new ResourceLibrary();
  m_window              = new ResourceLibraryWindow(m_library, mainWindow);
  m_resourceItemFactory = new ResourceItemFactory();
  m_windowGroupAdder    = new ResourceLibraryWindowGroupAdder();

  // add to pool
  bool result = ObjectPool::Instance()->addObject(m_window) && ObjectPool::Instance()->addObject(m_resourceItemFactory) &&
                ObjectPool::Instance()->addObject(m_library) && ObjectPool::Instance()->addObject(m_windowGroupAdder);
  if (result)
  {
    // initial placement
    mainWindow->addDockWidget(Qt::LeftDockWidgetArea, m_window);

    // connect for notifications
    connect(mainWindow, SIGNAL(saveData(QXmlStreamWriter&)), m_library, SLOT(onSaveData(QXmlStreamWriter&)));
    connect(mainWindow, SIGNAL(loadData(QXmlStreamReader&)), m_library, SLOT(onLoadData(QXmlStreamReader&)));
    connect(m_library, SIGNAL(loaded(int)), m_window, SLOT(onLibraryLoaded(int)));
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

  if (NULL != m_windowGroupAdder)
  {
    // remove from pool
    ObjectPool::Instance()->removeObject(m_windowGroupAdder);

    // delete
    delete m_windowGroupAdder;
    m_windowGroupAdder = NULL;
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

  if (NULL != m_library)
  {
    // remove from pool
    ObjectPool::Instance()->removeObject(m_library);

    // delete
    delete m_library;
    m_library = NULL;
  }

  // clean up resources
  Q_CLEANUP_RESOURCE(resources);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_EXPORT_PLUGIN(ResouceLibraryPlugin)
