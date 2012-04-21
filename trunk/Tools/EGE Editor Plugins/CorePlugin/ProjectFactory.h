#ifndef PROJECT_FACTORY_H
#define PROJECT_FACTORY_H

/* Project factory object. It is possible to register custom project types with the factory. It can also serve as data model.
 */

#include <QString>
#include <QMap>
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include "coreplugin_global.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Project;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef Project* (*PFPROJECTCREATEFUNC) (QObject* parent, const QString& name, const QString& path);
typedef QString  (*PFPROJECTTYPENAMEFUNC) ();
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class COREPLUGIN_API ProjectFactory : public QAbstractItemModel
{
  Q_OBJECT

  public:

    ProjectFactory(QObject* parent = NULL);
   ~ProjectFactory();

    /* Registeres custom project type. */
    bool registerProject(PFPROJECTTYPENAMEFUNC typeNameFunc, PFPROJECTCREATEFUNC createFunc);
    /* Creates instance of project of the type given by name. */
    Project* createProject(const QString& typeName, const QString& name, const QString& path, QObject* parent) const;
    /* Returns TRUE if given project type is registered. */
    bool isProjectRegistered(const QString& typeName) const;

  private:

    /* QAbstractItemModel override. Returns the parent of the model item with the given index. If the item has no parent, an invalid QModelIndex is returned. */
    QModelIndex parent(const QModelIndex &index) const override;
    /* QAbstractItemModel override. Returns the index of the item in the model specified by the given row, column and parent index. */
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    /* QAbstractItemModel override. Returns the data stored under the given role for the item referred to by the index. */
    QVariant data(const QModelIndex &index, int role) const override;
    /* QAbstractItemModel override. Returns the number of columns for the children of the given parent. */
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    /* QAbstractItemModel override. Returns the number of rows under the given parent. */
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

#endif // PROJECT_FACTORY_H