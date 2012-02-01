#include "ResourceLibraryItemDelegate.h"
#include "ResourceLibraryItem.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceLibraryItemDelegate::ResourceLibraryItemDelegate(QWidget* parent) : QStyledItemDelegate(parent)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceLibraryItemDelegate::~ResourceLibraryItemDelegate()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! QStyledItemDelegate override. Renders the delegate using the given painter and style option for the item specified by index. */
void ResourceLibraryItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
  QStyledItemDelegate::paint(painter, option, index);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! QStyledItemDelegate override. Returns the size needed by the delegate to display the item specified by index, taking into account the style information 
     provided by option. */
QSize ResourceLibraryItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
  ResourceLibraryItem* item = static_cast<ResourceLibraryItem*>(index.internalPointer());

  // process according to type
  QSize size;
  switch (item->type())
  {
    case ResourceLibraryItem::TYPE_CONTAINER:

      size = QSize(200, 20);
      break;

    default:

      Q_ASSERT("Type not supported");
  }

  return size;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
