#include "ResourceLibraryItemDelegate.h"
#include "Resources/ResourceItem.h"
#include "Resources/ResourceItemContainer.h"
#include "Resources/ResourceItemImage.h"
#include "ResourceLibraryDataModel.h"
#include <QPainter>
#include <QTreeView>
#include <QLineEdit>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define ICON_OFFSET 2
#define CONTAINER_TYPE_TEXT_OFFSET 5
#define THUMBNAIL_SIZE 32
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceLibraryItemDelegate::ResourceLibraryItemDelegate(QObject* parent) : QStyledItemDelegate(parent),
                                                                            m_view(NULL)
{
  m_containerTypePixmap.load(":/small:folder");
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceLibraryItemDelegate::~ResourceLibraryItemDelegate()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! QStyledItemDelegate override. Renders the delegate using the given painter and style option for the item specified by index. */
void ResourceLibraryItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
  Q_ASSERT(m_view);

  ResourceItem* item = static_cast<ResourceItem*>(index.internalPointer());

  // store painter state
  painter->save();

  // draw background
  m_view->style()->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter, m_view);

  // process according to type
  if ("container" == item->type())
  {
    paint(painter, option, reinterpret_cast<ResourceItemContainer*>(item));
  }
  else if ("image" == item->type())
  {
    paint(painter, option, reinterpret_cast<ResourceItemImage*>(item));
  }

  // restore painter state to original
  painter->restore();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Paint container type. */
void ResourceLibraryItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const ResourceItemContainer* item) const
{
  QRect rect(option.rect.x() + ICON_OFFSET, option.rect.y() + ((option.rect.height() - m_containerTypePixmap.size().height()) >> 1), 
             m_containerTypePixmap.size().width(), m_containerTypePixmap.size().height());
  painter->drawPixmap(rect.intersect(option.rect), m_containerTypePixmap);

  rect.translate(option.rect.height() + CONTAINER_TYPE_TEXT_OFFSET, 0);
  rect.setWidth(option.fontMetrics.width(item->name()));
  painter->drawText(rect.intersect(option.rect), Qt::AlignCenter, item->name());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Paint image type. */
void ResourceLibraryItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const ResourceItemImage* item) const
{
  int thumbSize = qMax(item->thumbnailImage().size().height(), THUMBNAIL_SIZE);

  QRect rect(option.rect.x() + ICON_OFFSET, option.rect.y(), thumbSize, thumbSize);
  painter->drawImage(rect.intersect(option.rect), item->thumbnailImage());

  rect.translate(THUMBNAIL_SIZE + CONTAINER_TYPE_TEXT_OFFSET, 0);
  rect.setWidth(qMax(option.fontMetrics.width(item->name()), option.fontMetrics.width(item->path())));
  painter->drawText(rect.intersect(option.rect), Qt::AlignTop, item->name());
  painter->drawText(rect.intersect(option.rect), Qt::AlignBottom, item->path());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! QStyledItemDelegate override. Returns the widget used to edit the item specified by index for editing. 
 *  The parent widget and style option are used to control how the editor widget appears.
 */
QWidget* ResourceLibraryItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	// create editor
	QWidget* editor = QStyledItemDelegate::createEditor(parent, option, index);
  
	return editor;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! QStyledItemDelegate override. Updates the editor for the item specified by index according to the style option given. */
void ResourceLibraryItemDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
  ResourceItem* item = static_cast<ResourceItem*>(index.internalPointer());

  // process according to type
  if ("container" == item->type())
  {
    editor->move(option.rect.x() + ICON_OFFSET + option.rect.height() + CONTAINER_TYPE_TEXT_OFFSET, option.rect.y());
    editor->resize(editor->size().width(), option.rect.size().height());
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets view. */
void ResourceLibraryItemDelegate::setView(QTreeView* view)
{
  m_view = view;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
