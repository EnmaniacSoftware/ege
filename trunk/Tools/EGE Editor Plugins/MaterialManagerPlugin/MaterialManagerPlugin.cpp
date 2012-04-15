#include "MaterialManagerPlugin.h"
#include "MaterialManagerWindow.h"
#include <Core.h>
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
  Core* core = Core::Instance();
  MainWindow* mainWindow = core->mainWindow();

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
  Core* core = Core::Instance();
  MainWindow* mainWindow = core->mainWindow();

  if (NULL != m_window)
  {
    ObjectPool::Instance()->removeObject(m_window);

    mainWindow->removeChildWindow(m_window);
    delete m_window;
    m_window = NULL;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_EXPORT_PLUGIN(MaterialManagerPlugin)

