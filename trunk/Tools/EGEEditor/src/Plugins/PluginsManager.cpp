#include "PluginsManager.h"
#include "IPlugin.h"
#include <QDir>
#include <QLibrary>
#include <QDebug>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PluginsManager::PluginsManager(QObject* parent) : QObject(parent)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PluginsManager::~PluginsManager()
{
  unloadPlugins();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Loads plugins. */
bool PluginsManager::loadPlugins()
{
  // go thru all plugin paths
  foreach (const QString& directory, m_paths)
  {
    // enumarate all files
    QDir dir(directory);
    dir.setFilter(QDir::Files | QDir::NoSymLinks);

    QFileInfoList fileList = dir.entryInfoList();
    foreach (const QFileInfo& file, fileList)
    {
      qDebug() << Q_FUNC_INFO << tr("Loading plugin: %1...").arg(file.absoluteFilePath());

      // load plugin
      QLibrary library(file.absoluteFilePath(), this);

      // resolve
      PFPLUGINDEPENDENCIES dependencies = (PFPLUGINDEPENDENCIES) library.resolve("ege_plugin_dependencies");
      PFPLUGINNAME name                 = (PFPLUGINNAME) library.resolve("ege_plugin_name");
      PFPLUGININSTANCE instance         = (PFPLUGININSTANCE) library.resolve("ege_plugin_instance");

      if ((NULL == dependencies) || (NULL == name) || (NULL == instance))
      {
        // error!
        qDebug() << Q_FUNC_INFO << tr("Could not resolve for plugin: %1...").arg(file.absoluteFilePath());
        continue;
      }

      PluginData* pluginData = new PluginData;
      if (NULL == pluginData)
      {
        // error!
        qDebug() << Q_FUNC_INFO << tr("Could not create plugin: %1...").arg(file.absoluteFilePath());
        continue;
      }

      pluginData->name         = name();
      pluginData->dependencies = QString(dependencies()).split(" ", QString::SkipEmptyParts);
      pluginData->instance     = qobject_cast<IPlugin*>(instance());

      m_plugins.insert(name(), pluginData);
    }
  }

  QList<PluginData*> queue = loadQueue();
  foreach (PluginData* plugin, queue)
  {
    plugin->instance->initialize();
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Unloads plugins. */
void PluginsManager::unloadPlugins()
{
  // deinitialize plugins in reverse order
  QList<PluginData*> queue = loadQueue();
  while (!queue.isEmpty())
  {
    PluginData* pluginData = queue.takeLast();

    if (m_plugins.contains(pluginData->name))
    {
      pluginData->instance->deinitialize();
    }
  }

  qDeleteAll(m_plugins);
  m_plugins.clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets plugin directories. */
void PluginsManager::setPluginPaths(const QStringList& paths)
{
  m_paths = paths;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Generates load queue from all plugins. */
QList<PluginsManager::PluginData*> PluginsManager::loadQueue() const
{
  QList<PluginsManager::PluginData*> queue;

  // go thru all plugins
  foreach (PluginData* pluginData, m_plugins)
  {
    QList<PluginsManager::PluginData*> loopQueue;
    loadQueue(pluginData, queue, loopQueue);
  }

  return queue;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Processes given plugin resolving its dependencies and updating load queue. */
bool PluginsManager::loadQueue(PluginData* plugin, QList<PluginsManager::PluginData*>& queue, QList<PluginsManager::PluginData*>& loopQueue) const
{
  // check if already in load queue
  if (queue.contains(plugin))
  {
    // done
    return true;
  }

  // check if circular dependancy found
  if (loopQueue.contains(plugin))
  {
    // error!
    return false;
  }

  // add to loop queue pool
  loopQueue << plugin;

  // resolve dependencies
  foreach (const QString& dep, plugin->dependencies)
  {
    PluginData* depPlugin = m_plugins.value(dep);
    if (!loadQueue(depPlugin, queue, loopQueue))
    {
      // error!
      return false;
    }
  }

  // add to queue
  queue << plugin;

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
