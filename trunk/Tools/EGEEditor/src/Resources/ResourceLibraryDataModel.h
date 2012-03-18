#ifndef RESOURCE_LIBRARY_DATA_MODEL_H
#define RESOURCE_LIBRARY_DATA_MODEL_H

/*! Resource library data model class. 
 */

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include "Serializer.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class ResourceItem;
class ResourceItemFactory;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceLibraryDataModel : public QAbstractItemModel, public ISerializer
{
  public:

    ResourceLibraryDataModel(QObject* parent = NULL);
   ~ResourceLibraryDataModel();

  public:

    /*! Available custom roles. */
    enum ResourceLibraryModelRole
    {
      TypeRole = Qt::UserRole,                  /*!< Resource item type. */
      PathRole                                  /*!< Resource item path. Only valid for certain types. */
    };

  public:

    /* Clears data. */
    void clear();

    /* QAbstractItemModel override. Sets the role data for the item at index to value.*/
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;

    /* Serializes into given stream. */
    bool serialize(QXmlStreamWriter& stream) const override;
    /* Unserializes from given data stream. */
    bool unserialize(QXmlStreamReader& stream) override;

    /* Removes entry associated with given index. */
    void removeItem(const QModelIndex& index);
    /* Inserts item after given index. */
    QModelIndex insertItem(const QModelIndex& index, ResourceItem* item);

  private:

    /* QAbstractItemModel override. Returns the parent of the model item with the given index. If the item has no parent, an invalid QModelIndex is returned. */
    QModelIndex parent(const QModelIndex& index) const override;
    /* QAbstractItemModel override. Returns the data stored under the given role for the item referred to by the index. */
    QVariant data(const QModelIndex& index, int role) const override;
    /* QAbstractItemModel override. Returns the number of rows under the given parent. */
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    /* QAbstractItemModel override. Returns the item flags for the given index. */
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    /* QAbstractItemModel override. Returns the number of columns for the children of the given parent. */
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    /* QAbstractItemModel override. Returns the index of the item in the model specified by the given row, column and parent index. */
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    /* Returns resource library item connected for given model index. */
    ResourceItem* getItem(const QModelIndex& index) const;
    /*! Returns root item. */
    inline ResourceItem* root() const { return m_root; }

  private:

    /*! Root item. */
    ResourceItem* m_root;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // RESOURCE_LIBRARY_DATA_MODEL_H