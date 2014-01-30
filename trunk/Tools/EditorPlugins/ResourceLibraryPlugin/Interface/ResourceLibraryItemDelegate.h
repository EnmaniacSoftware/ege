#ifndef RESOURCELIBRARY_RESOURCELIBRARYITEMDELEGATE_H
#define RESOURCELIBRARY_RESOURCELIBRARYITEMDELEGATE_H

/*! Resource item render delegate. This is base class for all more sophosticated delegates.
 */

#include <QtWidgets/QStyledItemDelegate>
#include "ResouceLibraryPlugin_global.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceItem;
class ResourceItemGroup;
class ResourceItemTexture;
class ResourceItemTextureAtlas;
class QAbstractItemView;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class RESOUCELIBRARYPLUGIN_API ResourceLibraryItemDelegate : public QStyledItemDelegate
{
  Q_OBJECT

  public:

    explicit ResourceLibraryItemDelegate(QObject* parent = NULL);
   ~ResourceLibraryItemDelegate();

    /*! Sets view. */
    void setView(QAbstractItemView* view);

  private:

    /*! @see QStyledItemDelegate::paint. */
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    /*! @see QStyledItemDelegate::createEditor. */
  	QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    /*! @see QStyledItemDelegate::sizeHint. */
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    /*! @see QStyledItemDelegate::updateEditorGeometry. */
    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

    /*! Paints group type resource item. */
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const ResourceItemGroup* item) const;
    /*! Paints texture type resource item. */
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const ResourceItemTexture* item) const;
    /*! Paints texture atlas type resource item. */
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const ResourceItemTextureAtlas* item) const;

    /*! Converts given model index into underlying model's index. */
    QModelIndex convertModelIndex(QModelIndex index) const;

  private:

    /*! View control where drawing takes place. */
    QAbstractItemView* m_view;
    /*! Container type pixmap. */
    QPixmap m_containerTypePixmap;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // RESOURCELIBRARY_RESOURCELIBRARYITEMDELEGATE_H
