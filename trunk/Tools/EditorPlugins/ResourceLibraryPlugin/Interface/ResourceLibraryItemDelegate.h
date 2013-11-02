#ifndef RESOURCELIBRARY_RESOURCELIBRARYITEMDELEGATE_H
#define RESOURCELIBRARY_RESOURCELIBRARYITEMDELEGATE_H

/*! Resource item render delegate. This is base class for all more sophosticated delegates.
 */

#include <QtWidgets/QStyledItemDelegate>
#include "ResouceLibraryPlugin_global.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceItem;
class ResourceItemContainer;
class ResourceItemImage;
class QTreeView;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class RESOUCELIBRARYPLUGIN_API ResourceLibraryItemDelegate : public QStyledItemDelegate
{
  Q_OBJECT

  public:

    explicit ResourceLibraryItemDelegate(QObject* parent = NULL);
   ~ResourceLibraryItemDelegate();

    /*! Sets view. */
    void setView(QTreeView* view);

  private:

    /*! @see QStyledItemDelegate::paint. */
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    /*! @see QStyledItemDelegate::createEditor. */
  	QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    /*! @see QStyledItemDelegate::sizeHint. */
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    /*! @see QStyledItemDelegate::updateEditorGeometry. */
    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

    /*! Paints container type. */
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const ResourceItemContainer* item) const;
    /*! Paints image type. */
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const ResourceItemImage* item) const;

  private:

    /*! View control where drawing takes place. */
    QTreeView* m_view;
    /*! Container type pixmap. */
    QPixmap m_containerTypePixmap;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // RESOURCELIBRARY_RESOURCELIBRARYITEMDELEGATE_H
