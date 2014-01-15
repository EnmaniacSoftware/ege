#include "NewProjectWindow.h"
#include "MainWindow.h"
#include <FileSystemUtils.h>
#include <Projects/ProjectFactory.h>
#include <Projects/Project.h>
#include <ObjectPool.h>
#include <QFileDialog.h>
#include <QMessageBox>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
NewProjectWindow::NewProjectWindow(QWidget* parent) : QDialog(parent)
{
  // setup UI
  setupUi(this);

  // set project location default value
  projectLocation->setText(QApplication::applicationDirPath());

  // populate project type list
  populateProjectTypeList();

  // update UI
  updateUI();

  // make connections
  connect(projectName, SIGNAL(textEdited(const QString&)), this, SLOT(projectNameTextEdited(const QString&)));
  //  connect(this, SIGNAL(projectCreated(Project*)), Core::Instance(), SLOT(onProjectCreated(Project*)));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
NewProjectWindow::~NewProjectWindow()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void NewProjectWindow::populateProjectTypeList()
{
  ProjectFactory* projectFactory = ObjectPool::Instance()->getObject<ProjectFactory>();
  Q_ASSERT(NULL != projectFactory);

  projectListView->setModel(projectFactory);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void NewProjectWindow::on_projectListView_clicked(const QModelIndex& index)
{
  Q_UNUSED(index);

  // update UI
  updateUI();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void NewProjectWindow::accept()
{
  ProjectFactory* projectFactory = ObjectPool::Instance()->getObject<ProjectFactory>();
  Q_ASSERT(NULL != projectFactory);

  QModelIndex index = projectListView->selectionModel()->selectedIndexes().first();
  if ( ! index.isValid())
  {
    // done
    return;
  }

  // check if project already exists at specified destination
  if (projectExists(projectName->text(), projectLocation->text()))
  {
    // prompt
    if (QMessageBox::No == QMessageBox::warning(this, windowTitle(), tr("Project already exists at given destination.\n\nDo you want to overwrite ?"),
                                                QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
    {
      // do nothing
      return;
    }
  }

  // create new project
  // NOTE: created object will add itself to pool
  Project* project = projectFactory->createProject(index.data(Qt::DisplayRole).toString(), projectName->text(), projectLocation->text(), NULL);
  Q_ASSERT(NULL != project);

  // add to object pool
  ObjectPool::Instance()->addObject(project);

  //  set dirty flag
  project->setDirty(true);

  // call base class
  QDialog::accept();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void NewProjectWindow::on_projectLocationBrowse_clicked()
{
  QString location = QFileDialog::getExistingDirectory(this, tr("Select project location"), projectLocation->text());
  if ( ! location.isEmpty())
  {
    projectLocation->setText(location);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool NewProjectWindow::projectExists(const QString& name, const QString& path) const
{
  return QFile::exists(path + "/" + name + ".egeproj");
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void NewProjectWindow::projectNameTextEdited(const QString& text)
{
  Q_UNUSED(text);

  // update UI
  updateUI();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void NewProjectWindow::updateUI()
{
  // get project selection model
  QItemSelectionModel* selectionModel = projectListView->selectionModel();

  // check if no selection
  if ((NULL == selectionModel) || selectionModel->selectedRows().empty())
  {
    // disable OK button
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    // disable project name field
    projectName->setEnabled(false);

    // disable project location section
    projectLocation->setEnabled(false);
    projectLocationBrowse->setEnabled(false);
  }
  else
  {
    // check if proper project name
    const QString currentName = projectName->text();
    const QString validatedName = FileSystemUtils::ValidateFileName(currentName);

    if ( ! validatedName.isEmpty() && (currentName == validatedName))
    {
      // enable OK button
      buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }
    else
    {
      // disable OK button
      buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    }

    // enable project name field
    projectName->setEnabled(true);

    // enable project location section
    projectLocation->setEnabled(true);
    projectLocationBrowse->setEnabled(true);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
