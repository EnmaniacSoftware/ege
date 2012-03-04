#ifndef RESOURCE_LIBRARY_ITEM_DELEGATE_H
#define RESOURCE_LIBRARY_ITEM_DELEGATE_H

#include <QStyledItemDelegate>
#include <QWidget>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class ResourceItem;
class ResourceItemContainer;
class ResourceItemImage;
class QTreeView;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! */
class ResourceLibraryItemDelegate : public QStyledItemDelegate
{
  Q_OBJECT

  public:

    ResourceLibraryItemDelegate(QObject* parent);
   ~ResourceLibraryItemDelegate();

    // Sets view. */
    void setView(QTreeView* view);

  private:

    /* QStyledItemDelegate override. Renders the delegate using the given painter and style option for the item specified by index. */
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    /* QStyledItemDelegate override. Returns the widget used to edit the item specified by index for editing. 
     * The parent widget and style option are used to control how the editor widget appears.
     */
  	QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    /* QStyledItemDelegate override. Returns the size needed by the delegate to display the item specified by index, taking into account the style information 
     * provided by option. 
     */
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    /* QStyledItemDelegate override. Updates the editor for the item specified by index according to the style option given. */
    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

    /* Paint container type. */
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const ResourceItemContainer* item) const;
    /* Paint image type. */
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const ResourceItemImage* item) const;

  private:

    /*! View control where drawing takes place. */
    QTreeView* m_view;
    /*! Container type pixmap. */
    QPixmap m_containerTypePixmap;

};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // RESOURCE_LIBRARY_ITEM_DELEGATE_H