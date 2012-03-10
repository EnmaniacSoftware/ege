#include "ProjectFactory.h"
#include "Project.h"
#include "JuiceJumpers/ProjectJuiceJumpers.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
struct BuiltInProject
{
  const char* name;
  projectCreateFunc pfCreateFunc;
};

static BuiltInProject l_projectsToRegister[] = { {"Juice Jumpers", ProjectJuiceJumpers::Create }
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ProjectFactory::ProjectFactory()
{
  // register build-in project types
  for (int i = 0; i < sizeof (l_projectsToRegister) / sizeof (BuiltInProject); ++i)
  {
    const BuiltInProject& item = l_projectsToRegister[i];

    registerProject(item.name, item.pfCreateFunc);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ProjectFactory::~ProjectFactory()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Registeres custom project type. */
bool ProjectFactory::registerProject(const QString& typeName, projectCreateFunc createFunc)
{
  // check if project with such a name exists already
  if (isProjectRegistered(typeName))
  {
    // error!
    return false;
  }

  // register
  ProjectData projectData;
  projectData.name       = typeName;
  projectData.createFunc = createFunc;

  m_registeredProjects.append(projectData);

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance of project of the type given by name. */
Project* ProjectFactory::createProject(const QString& typeName, const QString& name, const QString& path, QObject* parent) const
{
  Project* project = NULL;

  // get project create function for a given type name
  foreach (const ProjectData& projectData, m_registeredProjects)
  {
    if (projectData.name == typeName)
    {
      // create instance
      project = projectData.createFunc(parent, name, path);
      break;
    }
  }

  return project;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if given project type is registered. */
bool ProjectFactory::isProjectRegistered(const QString& typeName) const
{
  // get project create function for a given type name
  foreach (const ProjectData& projectData, m_registeredProjects)
  {
    if (projectData.name == typeName)
    {
      // found
      return true;
    }
  }
  
  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! QAbstractItemModel override. Returns the parent of the model item with the given index. If the item has no parent, an invalid QModelIndex is returned. */
QModelIndex ProjectFactory::parent(const QModelIndex& index) const
{
  Q_UNUSED(index);

  return QModelIndex();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! QAbstractItemModel override. Returns the index of the item in the model specified by the given row, column and parent index. */
QModelIndex ProjectFactory::index(int row, int column, const QModelIndex& parent) const
{
  return createIndex(row, column, NULL);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! QAbstractItemModel override. Returns the data stored under the given role for the item referred to by the index. */
QVariant ProjectFactory::data(const QModelIndex& index, int role) const
{
  // check if invalid index
  if (!index.isValid() || (index.row() >= m_registeredProjects.count()))
  {
    // done
    return QVariant();
  }

  if (Qt::DisplayRole != role)
  {
    // done
    return QVariant();
  }

  return m_registeredProjects[index.row()].name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! QAbstractItemModel override. Returns the number of columns for the children of the given parent. */
int ProjectFactory::columnCount(const QModelIndex& parent) const
{
  return 1;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! QAbstractItemModel override. Returns the number of rows under the given parent. */
int ProjectFactory::rowCount(const QModelIndex& parent) const
{
  return m_registeredProjects.count();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
