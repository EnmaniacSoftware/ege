#include "Plugins/PluginsManager.h"
#include "Plugins/Plugin.h"
#include <QDir>
#include <QLibrary>
#include <QDebug>
#include <QXmlStreamReader>

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
bool PluginsManager::loadPlugins()
{
  // enumarate all files
  QDir dir(pluginPath());
  dir.setFilter(QDir::Files | QDir::NoSymLinks);
  dir.setNameFilters(QStringList() << "*.xml");

  QFileInfoList fileList = dir.entryInfoList();
  foreach (const QFileInfo& fileInfo, fileList)
  {
    qDebug() << Q_FUNC_INFO << QString("Loading plugin definition: %1...").arg(fileInfo.absoluteFilePath());

    QFile file(fileInfo.absoluteFilePath());
    if ( ! file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
      // error!
      qWarning() << Q_FUNC_INFO << "Could not open plugin xml file" << fileInfo.absoluteFilePath();
      continue;
    }

    // allocate plugin data
    PluginData* pluginData = new PluginData;
    if (NULL == pluginData)
    {
      // error!
      qWarning() << Q_FUNC_INFO << QString("Could not create plugin: %1...").arg(fileInfo.absoluteFilePath());
      continue;
    }

    // process input data
    QXmlStreamReader stream(&file);
    while ( ! stream.atEnd())
    {
      QXmlStreamReader::TokenType token = stream.readNext();
      switch (token)
      {
        case QXmlStreamReader::StartElement:

          // check if plugin element
          if (QString::fromLatin1("plugin") == stream.name())
          {
            // retrive name
            pluginData->name = stream.attributes().value(QString::fromLatin1("name")).toString();
          }
          // check if dependency element
          else if (QString::fromLatin1("dependency") == stream.name())
          {
            // retrieve dependency name
            pluginData->dependencies.append(stream.attributes().value(QString::fromLatin1("name")).toString());
          }
          break;
      }
    }

    if (stream.hasError())
    {
      // error!
      qWarning() << Q_FUNC_INFO << "Plugin XML read error" <<  fileInfo.absoluteFilePath();
      delete pluginData;
      continue;
    }

    pluginData->instance  = NULL;
    pluginData->path      = dir.absolutePath();

    // add to pool
    m_plugins.insert(pluginData->name, pluginData);
  }

  QList<PluginData*> queue = loadQueue();
  foreach (PluginData* pluginData, queue)
  {
    // load plugin
    const QString libraryFullPath = QDir::toNativeSeparators(pluginData->path + QDir::separator() + pluginData->name);
    QLibrary library(libraryFullPath, this);
      
    // resolve
    PFPLUGININSTANCE instance = reinterpret_cast<PFPLUGININSTANCE>(library.resolve("ege_plugin_instance"));
    if (NULL == instance)
    {
      // error!
      qDebug() << Q_FUNC_INFO << QString("Could not resolve for plugin: %1...").arg(libraryFullPath);
      continue;
    }
    else
    {
      qDebug() << Q_FUNC_INFO << QString("Plugin instance created: %1...").arg(libraryFullPath);
    }

    // create instance
    pluginData->instance = qobject_cast<IPlugin*>(instance());
    if (NULL != pluginData->instance)
    {
      pluginData->instance->initialize();
    }
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PluginsManager::unloadPlugins()
{
  // deinitialize plugins in reverse order
  QList<PluginData*> queue = loadQueue();
  while ( ! queue.isEmpty())
  {
    PluginData* pluginData = queue.takeLast();

    if ((NULL != pluginData->instance) && m_plugins.contains(pluginData->name))
    {
      pluginData->instance->deinitialize();
    }
  }

  qDeleteAll(m_plugins);
  m_plugins.clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PluginsManager::setPluginPath(const QString& path)
{
  m_path = path;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
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
    Q_ASSERT(depPlugin);

    if ( ! loadQueue(depPlugin, queue, loopQueue))
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
const QString&PluginsManager::pluginPath() const
{
  return m_path;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
