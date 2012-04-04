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
      fpPluginDependencies dependencies = (fpPluginDependencies) library.resolve("ege_plugin_dependencies");
      fpPluginName name                 = (fpPluginName) library.resolve("ege_plugin_name");
      fpPluginInstance instance         = (fpPluginInstance) library.resolve("ege_plugin_instance");

      if ((NULL == dependencies) || (NULL == name) || (NULL == instance))
      {
        // error!
        qDebug() << Q_FUNC_INFO << tr("Could not resolve for plugin: %1...").arg(file.absoluteFilePath());
      }

      PluginData pluginData;
      pluginData.dependencies = QString(dependencies()).split(" ");
      pluginData.name         = name();
      //pluginData.instance     = qobject_cast<IPlugin*>(instance());

      m_plugins << pluginData;

      //QObject* pluginObj = loader.instance();
      //if (pluginObj)
      //{
      //  IPlugin* plugin = qobject_cast<IPlugin*>(pluginObj);
      //  if (plugin)
      //  {
      //    qDebug() << Q_FUNC_INFO << tr("Initializing plugin: %1...").arg(file.absoluteFilePath());
    
      //    // initialize
      //    if (!plugin->initialize())
      //    {
      //      qDebug() << Q_FUNC_INFO << tr("ERROR: Could not initialize!");
      //      loader.unload();
      //    }
      //    else
      //    {
      //      // add to pool
      //     // m_plugins << plugin;
      //    }
      //  }
      //  else
      //  {
      //    qDebug() << Q_FUNC_INFO << tr("ERROR: Casting failed for plugin %1!").arg(file.absoluteFilePath());
      //    loader.unload();
      //  }
      //}
      //else
      //{
      //  qDebug() << Q_FUNC_INFO << tr("ERROR: Could not load plugin %1!").arg(file.absoluteFilePath());
      //}
    }
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Unloads plugins. */
void PluginsManager::unloadPlugins()
{
  //foreach (IPlugin* plugin, m_plugins)
  //{
  //  plugin->deinitialize();
  //}
  m_plugins.clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets plugin directories. */
void PluginsManager::setPluginPaths(const QStringList& paths)
{
  m_paths = paths;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
