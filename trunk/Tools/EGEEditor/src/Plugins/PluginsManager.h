#ifndef PLUGINS_MANAGER_H
#define PLUGINS_MANAGER_H

#include <QStringList>
#include <List>

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

    /*! Plugin paths. */
    QStringList m_paths;
    /*! List of all plugins. */
    QList<PluginData> m_plugins;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // PLUGINS_MANAGER_H