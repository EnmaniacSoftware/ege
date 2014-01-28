#ifndef WORKSPACE_DIRECTORYPATHEDITWIDGET_H
#define WORKSPACE_DIRECTORYPATHEDITWIDGET_H

/*! Editor widget used to edit QtProperty values of type DirectoryPath.
 *  Instances of this class are created by DirectoryPathEditFactory.
 */

#include <QWidget>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
namespace Ui
{
  class FilePathEditWidget;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class DirectoryPathEditWidget : public QWidget
{
  Q_OBJECT

  public:

     DirectoryPathEditWidget(const QString& title, QWidget* parent = 0);
    ~DirectoryPathEditWidget();

  signals:

    /*! Signal emitted when Directory path has changed. */
    void directoryPathChanged(const QString& path);

  public:

    /*! Sets path. */
    void setPath(const QString& path);

  private slots:

    /*! Slot called when browse button has been clicked. */
    void onBrowse();

  private:

    /*! UI. */
    Ui::FilePathEditWidget* m_ui;
    /*! Browse window title. */
    QString m_title;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // WORKSPACE_DIRECTORYPATHEDITWIDGET_H
