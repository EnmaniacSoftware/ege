#include "ResourceLibraryItemDelegate.h"
#include "ResourceLibraryItem.h"
#include "ResourceLibraryDataModel.h"
#include <QPainter>
#include <QTreeView>

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
  ResourceLibraryItem* item      = static_cast<ResourceLibraryItem*>(index.internalPointer());
  ResourceLibraryItem::Type type = static_cast<ResourceLibraryItem::Type>(index.data(ResourceLibraryDataModel::TypeRole).toInt());

  // call base class first
  QStyledItemDelegate::paint(painter, option, index);

  // process according to type
  if (ResourceLibraryItem::TYPE_CONTAINER == type)
  {
    paintContainer(painter, option, item);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! QStyledItemDelegate override. 
    Returns the size needed by the delegate to display the item specified by index, taking into account the style information provided by option. */
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
/*! Paint container type. */
void ResourceLibraryItemDelegate::paintContainer(QPainter* painter, const QStyleOptionViewItem& option, const ResourceLibraryItem* item) const
{
  QPixmap icon(":/icons/add.ico");
  
  QRect rect(option.rect.x(), option.rect.y() + (option.rect.height() - option.decorationSize.height()) >> 1, option.decorationSize.width(), option.decorationSize.height());
  painter->drawPixmap(rect.intersect(option.rect), icon);

  rect.translate(option.rect.height() + 5, 0);
  rect.setWidth(option.fontMetrics.width(item->name()));
  painter->drawText(rect.intersect(option.rect), item->name());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
