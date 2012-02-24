#ifndef RESOURCE_LIBRARY_DATA_MODEL_H
#define RESOURCE_LIBRARY_DATA_MODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include "Project.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class ResourceItem;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceLibraryDataModel : public QAbstractItemModel
{
  public:

    ResourceLibraryDataModel(QObject* parent = NULL);
   ~ResourceLibraryDataModel();

  public:

    /*! Available custom roles. */
    enum ResourceLibraryModelRole
    {
      TypeRole = Qt::UserRole,
      PathRole
    };

  public:

    /* Loads data from given string. */
    bool load(const QString& data);
    /* Saves data to string. */
    QString save() const;
    /* Clears data. */
    void clear();
    /* Adds */
    void add(ResourceItem* item);
    /* Creates default. */
    void createDefault();
    /*! Returns root item. */
    inline ResourceItem* root() const { return m_root; }

    /* QAbstractItemModel override. Returns the number of columns for the children of the given parent. */
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    /* QAbstractItemModel override. Returns the index of the item in the model specified by the given row, column and parent index. */
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    /* QAbstractItemModel override. Sets the role data for the item at index to value.*/
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
    /* QAbstractItemModel override. On models that support this, inserts count rows into the model before the given row. 
     * Items in the new row will be children of the item represented by the parent model index.
     */
    bool insertRows(int position, int rows, const QModelIndex& parent = QModelIndex()) override;

  private:

    /* QAbstractItemModel override. Returns the parent of the model item with the given index. If the item has no parent, an invalid QModelIndex is returned. */
    QModelIndex parent(const QModelIndex& index) const override;
    /* QAbstractItemModel override. Returns the data stored under the given role for the item referred to by the index. */
    QVariant data(const QModelIndex& index, int role) const override;
    /* QAbstractItemModel override. Returns the number of rows under the given parent. */
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    /* QAbstractItemModel override. Returns the item flags for the given index. */
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    /* QAbstractItemModel override. */
    bool removeRows(int position, int rows, const QModelIndex& parent = QModelIndex()) override;
    /* Returns resource library item connected for given model index. */
    ResourceItem* getItem(const QModelIndex& index) const;

  private:

    /*! Root item. */
    ResourceItem* m_root;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // RESOURCE_LIBRARY_DATA_MODEL_H