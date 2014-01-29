#include "ResourceLibraryItemDelegate.h"
#include "ResourceItem.h"
#include "ResourceItemGroup.h"
#include "ResourceItemTexture.h"
#include "ResourceItemTextureAtlas.h"
#include "ResourceLibraryDataModel.h"
#include <QPainter>
#include <QTreeView>
#include <QLineEdit>
#include <QAbstractProxyModel>
#include <ObjectPool.h>
#include <QDebug>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const int KIconOffset              = 2;
static const int KContainerTypeTextOffset = 5;
static const int KThumbnailSize           = 32;
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

  ResourceItem* item = static_cast<ResourceItem*>(convertModelIndex(index).internalPointer());

  // store painter state
  painter->save();

  // draw background
  m_view->style()->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter, m_view);

  // process according to type
  if (ResourceItemGroup::TypeName() == item->typeName())
  {
    paint(painter, option, reinterpret_cast<ResourceItemGroup*>(item));
  }
  else if (ResourceItemTexture::TypeName() == item->typeName())
  {
    paint(painter, option, reinterpret_cast<ResourceItemTexture*>(item));
  }
  else if (ResourceItemTextureAtlas::TypeName() == item->typeName())
  {
    paint(painter, option, reinterpret_cast<ResourceItemTextureAtlas*>(item));
  }

  // restore painter state to original
  painter->restore();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceLibraryItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const ResourceItemGroup* item) const
{
  QRect rect(option.rect.x() + KIconOffset, option.rect.y() + ((option.rect.height() - m_containerTypePixmap.size().height()) >> 1),
             m_containerTypePixmap.size().width(), m_containerTypePixmap.size().height());
  painter->drawPixmap(rect.intersected(option.rect), m_containerTypePixmap);

  rect.translate(option.rect.height() + KContainerTypeTextOffset, 0);
  rect.setWidth(option.fontMetrics.width(item->name()));
  painter->drawText(rect.intersected(option.rect), Qt::AlignCenter, item->name());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceLibraryItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const ResourceItemTexture* item) const
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
  rect.setWidth(qMax(option.fontMetrics.width(item->name()), option.fontMetrics.width(item->fullPath())));
  painter->drawText(rect.intersected(option.rect), Qt::AlignTop, item->name());
  painter->drawText(rect.intersected(option.rect), Qt::AlignBottom, item->fullPath());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceLibraryItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const ResourceItemTextureAtlas* item) const
{
  paint(painter, option, qobject_cast<const ResourceItemTexture*>(item));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QModelIndex ResourceLibraryItemDelegate::convertModelIndex(QModelIndex index) const
{
  const QAbstractProxyModel* proxyModel = qobject_cast<const QAbstractProxyModel*>(index.model());

  // check if index is part of any proxy model
  if (NULL != proxyModel)
  {
    // convert index to source
    index = proxyModel->mapToSource(index);
  }

  return index;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QWidget* ResourceLibraryItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
  Q_UNUSED(parent);
  Q_UNUSED(option);
  Q_UNUSED(index);

  // no editing at this level is possible
  return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//void ResourceLibraryItemDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
//{
//  ResourceItem* item = static_cast<ResourceItem*>(convertModelIndex(index).internalPointer());

//  // process according to type
//  if (ResourceItemGroup::TypeName() == item->typeName())
//  {
//    editor->move(option.rect.x() + KIconOffset + option.rect.height() + KContainerTypeTextOffset, option.rect.y());
//    editor->resize(editor->size().width(), option.rect.size().height());
//  }
//}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceLibraryItemDelegate::setView(QAbstractItemView* view)
{
  m_view = view;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QSize ResourceLibraryItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
  Q_UNUSED(option);

  ResourceItem* item = static_cast<ResourceItem*>(convertModelIndex(index).internalPointer());

  return (NULL != item) ? item->sizeHint() : QStyledItemDelegate::sizeHint(option, index);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
