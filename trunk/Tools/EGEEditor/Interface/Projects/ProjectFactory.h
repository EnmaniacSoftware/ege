#ifndef EDITOR_PROJECTS_PROJECTFACTORY_H
#define EDITOR_PROJECTS_PROJECTFACTORY_H

/*! Project factory object.
 *  It is possible to register custom project types with the factory.
 *  It can also serve as data model.
 */

#include <QString>
#include <QMap>
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Project;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef Project* (*PFPROJECTCREATEFUNC) (QObject* parent, const QString& name, const QString& path);
typedef QString  (*PFPROJECTTYPENAMEFUNC) ();
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ProjectFactory : public QAbstractItemModel
{
  Q_OBJECT

  public:

    ProjectFactory(QObject* parent = NULL);
   ~ProjectFactory();

    /*! Registeres custom project type. */
    bool registerProject(PFPROJECTTYPENAMEFUNC typeNameFunc, PFPROJECTCREATEFUNC createFunc);
    /*! Creates instance of project of the type given by name. */
    Project* createProject(const QString& typeName, const QString& name, const QString& path, QObject* parent) const;
    /*! Returns TRUE if given project type is registered. */
    bool isProjectRegistered(const QString& typeName) const;

  private:

    /*! @see QAbstractItemModel::parent. */
    QModelIndex parent(const QModelIndex &index) const override;
    /*! @see QAbstractItemModel::index. */
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    /*! @see QAbstractItemModel::data. */
    QVariant data(const QModelIndex &index, int role) const override;
    /*! @see QAbstractItemModel::columnCount. */
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    /*! @see QAbstractItemModel::rowCount. */
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

  private:

    /*! Data struct of registered project. */
    struct ProjectData
    {
      PFPROJECTTYPENAMEFUNC typeNameFunc; /*!< Project type name function. */
      PFPROJECTCREATEFUNC createFunc;     /*!< Project create function. */
    };

    typedef QList<ProjectData> ProjectRegisterList;

  private:

    /*! List of registered projects. */
    ProjectRegisterList m_registeredProjects;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EDITOR_PROJECTS_PROJECTFACTORY_H
