#ifndef NEW_PROJECT_DIALOG_H
#define NEW_PROJECT_DIALOG_H

#include <QDialog>
#include <QModelIndex>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Ui_NewProjectDialog;
class Project;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class NewProjectDialog : public QDialog
{
  Q_OBJECT

  public:
    
    NewProjectDialog(QWidget* parent = NULL);
   ~NewProjectDialog();

  signals:

    /*! Signal emitted when valid project has been selected. 
     *  @param  Pointer to newly created project.
     */
    void projectCreated(Project* project);

  private slots:

    /* Slot called when item in the list is clicked. */
    void on_projectListView_clicked(const QModelIndex& index);
    /* Slot called when location browse button is clicked. */
    void on_projectLocationBrowse_clicked();
    /* Slot called when OK button is clicked. */
    void accept();

  private:

    /* Populates project type list. */
    void populateProjectTypeList();

  private:

    /*! Dialog UI. */
    Ui_NewProjectDialog* m_ui;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // NEW_PROJECT_DIALOG_H