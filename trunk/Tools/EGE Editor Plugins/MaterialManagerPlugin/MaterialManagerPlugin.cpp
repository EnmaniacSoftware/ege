#include "MaterialManagerPlugin.h"
#include "MaterialManagerWindow.h"
#include <MainWindow.h>
#include <QtPlugin>
#include <QDebug>
#include <ObjectPool.h>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
MaterialManagerPlugin::MaterialManagerPlugin(QObject* parent) : QObject(parent),
                                                                m_window(NULL)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
MaterialManagerPlugin::~MaterialManagerPlugin()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IPlugin override. Initialized plugin. */
bool MaterialManagerPlugin::initialize()
{
  MainWindow* mainWindow = ObjectPool::Instance()->getObject<MainWindow>();
  if (!mainWindow)
  {
    // error!
    qWarning() << Q_FUNC_INFO << "No MainWindow found!";
    return false;
  }
 
  m_window = new MaterialManagerWindow();
  if (NULL != m_window)
  {
    mainWindow->addChildWindow(m_window);

    return ObjectPool::Instance()->addObject(m_window);
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IPlugin override. Deinitializes plugin. */
void MaterialManagerPlugin::deinitialize()
{
  if (NULL != m_window)
  {
    //ObjectPool::Instance()->removeObject(m_window);

    MainWindow* mainWindow = ObjectPool::Instance()->getObject<MainWindow>();
    if (mainWindow)
    {
      mainWindow->removeChildWindow(m_window);
    }

    delete m_window;
    m_window = NULL;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_EXPORT_PLUGIN(MaterialManagerPlugin)

