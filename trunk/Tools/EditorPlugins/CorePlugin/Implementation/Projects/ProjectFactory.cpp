#include "Projects/ProjectFactory.h"
#include "Projects/Project.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ProjectFactory::ProjectFactory(QObject* parent) : QAbstractItemModel(parent)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ProjectFactory::~ProjectFactory()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ProjectFactory::registerProject(PFPROJECTTYPENAMEFUNC typeNameFunc, PFPROJECTCREATEFUNC createFunc)
{
  Q_ASSERT((NULL != typeNameFunc) && (NULL != createFunc));

  // check if project with such a name exists already
  if (isProjectRegistered(typeNameFunc()))
  {
    // error!
    return false;
  }

  // register
  ProjectData projectData;
  projectData.typeNameFunc  = typeNameFunc;
  projectData.createFunc    = createFunc;

  m_registeredProjects.append(projectData);

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Project* ProjectFactory::createProject(const QString& typeName, const QString& name, const QString& path, QObject* parent) const
{
  Project* project = NULL;

  // get project create function for a given type name
  foreach (const ProjectData& projectData, m_registeredProjects)
  {
    if (projectData.typeNameFunc() == typeName)
    {
      // create instance
      project = projectData.createFunc(parent, name, path);
      break;
    }
  }

  return project;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ProjectFactory::isProjectRegistered(const QString& typeName) const
{
  foreach (const ProjectData& projectData, m_registeredProjects)
  {
    if (projectData.typeNameFunc() == typeName)
    {
      // found
      return true;
    }
  }
  
  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QModelIndex ProjectFactory::parent(const QModelIndex& index) const
{
  Q_UNUSED(index);

  return QModelIndex();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QModelIndex ProjectFactory::index(int row, int column, const QModelIndex& parent) const
{
  return createIndex(row, column);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QVariant ProjectFactory::data(const QModelIndex& index, int role) const
{
  // check if invalid index
  if ( ! index.isValid() || (index.row() >= m_registeredProjects.count()))
  {
    // done
    return QVariant();
  }

  if (Qt::DisplayRole != role)
  {
    // done
    return QVariant();
  }

  return m_registeredProjects[index.row()].typeNameFunc();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
int ProjectFactory::columnCount(const QModelIndex& parent) const
{
  return 1;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
int ProjectFactory::rowCount(const QModelIndex& parent) const
{
  return m_registeredProjects.count();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
