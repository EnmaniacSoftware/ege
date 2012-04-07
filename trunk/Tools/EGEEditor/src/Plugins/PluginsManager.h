#ifndef PLUGINS_MANAGER_H
#define PLUGINS_MANAGER_H

#include <QStringList>
#include <QMap>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class IPlugin;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class PluginsManager : public QObject
{
  public:

    explicit PluginsManager(QObject* parent = NULL);
    ~PluginsManager();

    /* Sets plugin directories. */
    void setPluginPaths(const QStringList& paths);

    /* Loads plugins. */
    bool loadPlugins();
    /* Unloads plugins. */
    void unloadPlugins();
    
  private:

    /*! Plugin data. */
    struct PluginData
    {
      QString name;
      QStringList dependencies;
      IPlugin* instance;
    };

  private:

    /* Generates load queue from all plugins. */
    QList<PluginData*> loadQueue() const;
    /* Processes given plugin resolving its dependencies and updating load queue. */
    bool loadQueue(PluginData* plugin, QList<PluginData*>& queue, QList<PluginsManager::PluginData*>& loopQueue) const;

  private:

    /*! Plugin paths. */
    QStringList m_paths;
    /*! Map of all plugins [name, plugindata]. */
    QMap<QString, PluginData*> m_plugins;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // PLUGINS_MANAGER_H