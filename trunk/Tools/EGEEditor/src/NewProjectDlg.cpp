#include "NewProjectDlg.h"
#include "NewProjectDataModel.h"
#include "Utils/FSUtils.h"
#include "ui_newproject.h"
#include <QFileDialog.h>
#include <QMessageBox>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
NewProjectDialog::NewProjectDialog(QWidget* parent) : QDialog(parent),
                                                      m_ui(new Ui_NewProjectDialog())
{
  m_ui->setupUi(this);

  // set project location default value
  m_ui->projectLocation->setText(QApplication::applicationDirPath());

  // populate project type list
  populateProjectTypeList();

  // initially disable OK button
  m_ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

  // make connections
  connect(m_ui->projectName, SIGNAL(textEdited(const QString&)), this, SLOT(projectNameTextEdited(const QString&)));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
NewProjectDialog::~NewProjectDialog()
{
  if (m_ui)
  {
    delete m_ui;
    m_ui = NULL;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Populates project type list. */
void NewProjectDialog::populateProjectTypeList()
{
  NewProjectDataModel* dataModel = new NewProjectDataModel(this);
  m_ui->projectListView->setModel(dataModel);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when item in the list is clicked. */
void NewProjectDialog::on_projectListView_clicked(const QModelIndex& index)
{
  Q_UNUSED(index);

  // update OK button
  updateOKButton();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when OK button is clicked. */
void NewProjectDialog::accept()
{
  QModelIndex index = m_ui->projectListView->selectionModel()->selectedIndexes().first();
  if (!index.isValid())
  {
    // done
    return;
  }

  // check if project already exists at specified destination
  if (projectExists(m_ui->projectName->text(), m_ui->projectLocation->text()))
  {
    // prompt
    if (QMessageBox::No == QMessageBox::warning(this, windowTitle(), tr("Project already exists at given destination.\n\nDo you want to overwrite ?"),
                                                QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
    {
      // do nothing
      return;
    }
  }

  NewProjectDataModel::ModelItem* modelItem = static_cast<NewProjectDataModel::ModelItem*>(index.internalPointer());

  // create new project
  Project* project = modelItem->createFunc(parent(), m_ui->projectName->text(), m_ui->projectLocation->text());

  // emit
  emit projectCreated(project);

  // call base class
  QDialog::accept();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when location browse button is clicked. */
void NewProjectDialog::on_projectLocationBrowse_clicked()
{
  QString location = QFileDialog::getExistingDirectory(this, tr("Select project location"), m_ui->projectLocation->text());
  if (!location.isEmpty())
  {
    m_ui->projectLocation->setText(location);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Checks if project already exists in a given directory. */
bool NewProjectDialog::projectExists(const QString& name, const QString& path) const
{
  return QFile::exists(path + "/" + name + ".ege");
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when project text name changes. */
void NewProjectDialog::projectNameTextEdited(const QString& text)
{
  Q_UNUSED(text);

  // update OK button
  updateOKButton();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates OK button. */
void NewProjectDialog::updateOKButton()
{
  // check if no selection or invalid project file name
  if (m_ui->projectListView->selectionModel()->selectedRows().empty() ||
      m_ui->projectName->text() != FSUtils::ValidateFileName(m_ui->projectName->text()))
  {
    // disable OK button
    m_ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
  }
  else
  {
    // enable OK button
    m_ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
