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

  private:

    /* QStyledItemDelegate override. Renders the delegate using the given painter and style option for the item specified by index. */
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    /* QStyledItemDelegate override. Returns the size needed by the delegate to display the item specified by index, taking into account the style information 
       provided by option. */
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // RESOURCE_LIBRARY_ITEM_DELEGATE_H