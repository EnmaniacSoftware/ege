#include "ResourceLibraryItemDelegate.h"
#include "ResourceItem.h"
#include "ResourceItemContainer.h"
#include "ResourceItemImage.h"
#include "ResourceLibraryDataModel.h"
#include <QPainter>
#include <QTreeView>
#include <QLineEdit>
#include <QDebug>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const int KIconOffset = 2;
static const int KContainerTypeTextOffset = 5;
static const int KThumbnailSize = 32;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceLibraryItemDelegate::ResourceLibraryItemDelegate(QObject* parent) : QStyledItemDelegate(parent),
                                                                            m_view(NULL)
{
  bool result = m_containerTypePixmap.load(":/resourcelibrary/folder");
  Q_ASSERT(result);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceLibraryItemDelegate::~ResourceLibraryItemDelegate()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceLibraryItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
  Q_ASSERT(m_view);

  ResourceItem* item = static_cast<ResourceItem*>(index.internalPointer());

  // store painter state
  painter->save();

  // draw background
  m_view->style()->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter, m_view);

  // process according to type
  if (ResourceItemContainer::TypeName() == item->type())
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
void ResourceLibraryItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const ResourceItemContainer* item) const
{
  QRect rect(option.rect.x() + KIconOffset, option.rect.y() + ((option.rect.height() - m_containerTypePixmap.size().height()) >> 1),
             m_containerTypePixmap.size().width(), m_containerTypePixmap.size().height());
  painter->drawPixmap(rect.intersected(option.rect), m_containerTypePixmap);

  rect.translate(option.rect.height() + KContainerTypeTextOffset, 0);
  rect.setWidth(option.fontMetrics.width(item->name()));
  painter->drawText(rect.intersected(option.rect), Qt::AlignCenter, item->name());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceLibraryItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const ResourceItemImage* item) const
{
  // determine thumbnail image size
  int thumbSize = qMax(item->thumbnailImage().size().height(), KThumbnailSize);

  // determine centered region where drawing is going to take place
  const int offset = (option.rect.height() - thumbSize) / 2;
  QRect rect(option.rect.x() + KIconOffset, option.rect.y() + offset, thumbSize, thumbSize);

  // paint thumbnail
  painter->drawImage(rect.intersected(option.rect), item->thumbnailImage());

  //qDebug() << rect << option.rect << rect.intersected(option.rect);

  // paint text fields
  rect.translate(KThumbnailSize + KContainerTypeTextOffset, 0);
  rect.setWidth(qMax(option.fontMetrics.width(item->name()), option.fontMetrics.width(item->path())));
  painter->drawText(rect.intersected(option.rect), Qt::AlignTop, item->name());
  painter->drawText(rect.intersected(option.rect), Qt::AlignBottom, item->path());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QWidget* ResourceLibraryItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	// create editor
	QWidget* editor = QStyledItemDelegate::createEditor(parent, option, index);
  
	return editor;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceLibraryItemDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
  ResourceItem* item = static_cast<ResourceItem*>(index.internalPointer());

  // process according to type
  if (ResourceItemContainer::TypeName() == item->type())
  {
    editor->move(option.rect.x() + KIconOffset + option.rect.height() + KContainerTypeTextOffset, option.rect.y());
    editor->resize(editor->size().width(), option.rect.size().height());
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceLibraryItemDelegate::setView(QTreeView* view)
{
  m_view = view;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QSize ResourceLibraryItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
  Q_UNUSED(option);

  ResourceItem* item = static_cast<ResourceItem*>(index.internalPointer());

  return item->sizeHint();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
