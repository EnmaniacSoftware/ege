#ifndef NEW_PROJECT_WINDOW_H
#define NEW_PROJECT_WINDOW_H

#include <QDialog>
#include <QModelIndex>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Ui_NewProjectDialog;
class Project;
class MainWindow;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class NewProjectWindow : public QDialog
{
  Q_OBJECT

  public:
    
    NewProjectWindow(QWidget* parent = NULL);
   ~NewProjectWindow();

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
    /* Slot called when project text name changes. */
    void projectNameTextEdited(const QString& text);

  private:

    /* Populates project type list. */
    void populateProjectTypeList();
    /* Checks if project already exists in a given directory. */
    bool projectExists(const QString& name, const QString& path) const;
    /* Updates OK button. */
    void updateOKButton();
    /*! Returns pointer to main window. */
    inline MainWindow* mainWindow() const { return reinterpret_cast<MainWindow*>(parent()); }

  private:

    /*! Dialog UI. */
    Ui_NewProjectDialog* m_ui;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // NEW_PROJECT_WINDOW_H