#ifndef RESOURCE_LIBRARY_ITEM_DELEGATE_H
#define RESOURCE_LIBRARY_ITEM_DELEGATE_H

#include <QStyledItemDelegate>
#include <QWidget>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceLibraryItemDelegate : public QStyledItemDelegate
{
  Q_OBJECT

  public:

    ResourceLibraryItemDelegate(QWidget* parent);
   ~ResourceLibraryItemDelegate();

};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // RESOURCE_LIBRARY_ITEM_DELEGATE_H