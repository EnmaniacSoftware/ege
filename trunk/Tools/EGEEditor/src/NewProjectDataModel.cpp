#include "NewProjectDataModel.h"
#include "JuiceJumpers/ProjectJuiceJumpers.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static NewProjectDataModel::ModelItem l_modelItems[] = { { "Juice Jumpers", ProjectJuiceJumpers::Create }
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
NewProjectDataModel::NewProjectDataModel(QObject* parent) : QAbstractItemModel(parent)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
NewProjectDataModel::~NewProjectDataModel()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! QAbstractItemModel override. Returns the parent of the model item with the given index. If the item has no parent, an invalid QModelIndex is returned. */
QModelIndex NewProjectDataModel::parent(const QModelIndex& index) const
{
  return QModelIndex();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! QAbstractItemModel override. Returns the index of the item in the model specified by the given row, column and parent index. */
QModelIndex NewProjectDataModel::index(int row, int column, const QModelIndex& parent) const
{
  ModelItem* item;

  // check if parent invalid
  if (!parent.isValid())
  {
    item = &l_modelItems[0];
  }
  else
  {
    item = static_cast<ModelItem*>(parent.internalPointer());
  }

  return createIndex(row, column, item);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! QAbstractItemModel override. Returns the data stored under the given role for the item referred to by the index. */
QVariant NewProjectDataModel::data(const QModelIndex& index, int role) const
{
  // check if invalid index
  if (!index.isValid())
  {
    // done
    return QVariant();
  }

  if (Qt::DisplayRole != role)
  {
    // done
    return QVariant();
  }

  ModelItem* modelItem = static_cast<ModelItem*>(index.internalPointer());
  return modelItem->name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! QAbstractItemModel override. Returns the number of columns for the children of the given parent. */
int NewProjectDataModel::columnCount(const QModelIndex& parent) const
{
  return 1;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! QAbstractItemModel override. Returns the number of rows under the given parent. */
int NewProjectDataModel::rowCount(const QModelIndex& parent) const
{
  return sizeof (l_modelItems) / sizeof (l_modelItems[0]);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
