#ifndef RESOURCE_LIBRARY_DATA_MODEL_H
#define RESOURCE_LIBRARY_DATA_MODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include "Project.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class ResourceLibraryItem;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceLibraryDataModel : public QAbstractItemModel
{
  friend class NewProjectDialog;

  public:

    ResourceLibraryDataModel(QObject* parent = NULL);
   ~ResourceLibraryDataModel();

    /* Loads data from file at given path. */
    bool load(const QString& path);
    /* Saves data to file at given path. */
    bool save(const QString& path);
    /* Clears data. */
    void clear();
    /* Adds */
    void add(ResourceLibraryItem* item);
    /* Creates default. */
    void createDefault();

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

    /*! Root item. */
    ResourceLibraryItem* m_root;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // RESOURCE_LIBRARY_DATA_MODEL_H