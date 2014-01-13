#ifndef WORKSPACE_FILEPATHEDITWIDGET_H
#define WORKSPACE_FILEPATHEDITWIDGET_H

/*! Editor widget used to edit QtProperty values of type FilePath.
 *  Instances of this class are created by FilePathEditFactory.
 */

#include <QWidget>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
namespace Ui
{
  class FilePathEditWidget;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class FilePathEditWidget : public QWidget
{
  Q_OBJECT

  public:

     FilePathEditWidget(const QString& title, QWidget* parent = 0);
    ~FilePathEditWidget();

  signals:

    /*! Signal emitted when file path has changed. */
    void filePathChanged(const QString& path);

  public:

    /*! Sets path. */
    void setPath(const QString& path);
    /*! Sets file filters. */
    void setFilters(const QString& filters);

  private slots:

    /*! Slot called when browse button has been clicked. */
    void onBrowse();

  private:

    /*! UI. */
    Ui::FilePathEditWidget* m_ui;
    /*! Current filters value. */
    QString m_filters;
    /*! Browse window title. */
    QString m_title;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // WORKSPACE_FILEPATHEDITWIDGET_H
