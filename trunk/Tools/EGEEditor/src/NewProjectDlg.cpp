#include "NewProjectDlg.h"
#include "NewProjectDataModel.h"
#include "ui_newproject.h"
#include <QFileDialog.h>

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
  // enable OK button
  m_ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
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
