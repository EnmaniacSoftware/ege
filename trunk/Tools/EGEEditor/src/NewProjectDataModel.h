#ifndef NEW_PROJECT_DATA_MODEL_H
#define NEW_PROJECT_DATA_MODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include "Project.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class NewProjectDataModel : public QAbstractItemModel
{
  friend class NewProjectDialog;

  public:

    NewProjectDataModel(QObject* parent = NULL);
   ~NewProjectDataModel();

  public:

    /*! Structure describing single model item. */
    struct ModelItem
    {
      QString           name;             /*!< Item (project) name. */
      projectCreateFunc createFunc;       /*!< Item (project) create function. */
    };

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
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // NEW_PROJECT_DATA_MODEL_H