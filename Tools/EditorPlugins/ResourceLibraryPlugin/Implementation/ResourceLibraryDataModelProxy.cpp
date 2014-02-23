#include "ResourceLibraryDataModelProxy.h"
#include "ResourceLibraryDataModel.h"
#include <ObjectPool.h>
#include <Configuration.h>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceLibraryDataModelProxy::ResourceLibraryDataModelProxy(QObject* parent) : QSortFilterProxyModel(parent)
{
  Configuration* configuration = ObjectPool::Instance()->getObject<Configuration>();
  Q_ASSERT(NULL != configuration);

  // connect
  connect(configuration, SIGNAL(changed(QString)), this, SLOT(onConfigurationChanged(QString)));
  connect(configuration, SIGNAL(nameChanged(QString,QString)), this, SLOT(onConfigurationNameChanged(QString,QString)));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceLibraryDataModelProxy::~ResourceLibraryDataModelProxy()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceLibraryDataModelProxy::onConfigurationChanged(const QString& name)
{
  // store new fliltering
  m_configName = name;

  // invalidate filter to propagate change
  invalidateFilter();

  // notify
  emit filterChanged();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceLibraryDataModelProxy::onConfigurationNameChanged(const QString& oldName, const QString& newName)
{
  // check if current filter changed
  if (m_configName == oldName)
  {
    // accept new name
    onConfigurationChanged(newName);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ResourceLibraryDataModelProxy::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const
{
  QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);

  QString configRole = sourceModel()->data(index, ResourceLibraryDataModel::ConfigRole).toString();

  return (configRole == m_configName);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
