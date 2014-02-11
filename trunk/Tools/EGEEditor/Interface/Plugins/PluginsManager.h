#ifndef EDITOR_PLUGINS_PLUGINSMANAGER_H
#define EDITOR_PLUGINS_PLUGINSMANAGER_H

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

    /*! Sets plugin directory.
     *  @param  path  Path where both XML definitions and binaries of plugins are be located.
     */
    void setPluginPath(const QString& path);

    /*! Loads plugins. */
    bool loadPlugins();
    /*! Unloads plugins. */
    void unloadPlugins();
    
  private:

    /*! Plugin data. */
    struct PluginData
    {
      QString name;
      QString path;
      QStringList dependencies;
      IPlugin* instance;
    };

  private:

    /*! Generates load queue from all plugins. */
    QList<PluginData*> loadQueue() const;
    /*! Processes given plugin resolving its dependencies and updating load queue. */
    bool loadQueue(PluginData* plugin, QList<PluginData*>& queue, QList<PluginsManager::PluginData*>& loopQueue) const;
    /*! Returns path to plugin directory. */
    const QString& pluginPath() const;

  private:

    /*! Plugin path. */
    QString m_path;
    /*! Map of all plugins [name, plugindata]. */
    QMap<QString, PluginData*> m_plugins;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EDITOR_PLUGINS_PLUGINSMANAGER_H
