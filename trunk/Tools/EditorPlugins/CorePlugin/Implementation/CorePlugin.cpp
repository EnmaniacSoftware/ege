#include "CorePlugin.h"
#include "ObjectPool.h"
#include "Settings.h"
#include "Projects/ProjectFactory.h"
#include <QtPlugin>
#include <QResource>
#include <QDebug>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
CorePlugin::CorePlugin(QObject* parent) : QObject(parent),
                                          m_projectFactory(NULL),
                                          m_settings(NULL)
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

  // create objectss
  m_projectFactory = new ProjectFactory();
  m_settings = new Settings();

  // add to pool
  bool result = ObjectPool::Instance()->addObject(m_projectFactory) && ObjectPool::Instance()->addObject(m_settings);

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void CorePlugin::deinitialize()
{
  if (NULL != m_projectFactory)
  {
    ObjectPool::Instance()->removeObject(m_projectFactory);

    delete m_projectFactory;
    m_projectFactory = NULL;
  }

  if (NULL != m_settings)
  {
    ObjectPool::Instance()->removeObject(m_settings);

    delete m_settings;
    m_settings = NULL;
  }

  // clean up resources
  Q_CLEANUP_RESOURCE(core);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_EXPORT_PLUGIN(CorePlugin)
