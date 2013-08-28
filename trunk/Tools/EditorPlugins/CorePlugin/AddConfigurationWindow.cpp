#include "AddConfigurationWindow.h"
#include "ui_addconfiguration.h"
#include <QPushButton>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AddConfigurationWindow::AddConfigurationWindow(QWidget* parent) : QDialog(parent),
                                                                  m_ui(new Ui_AddConfiguration())
{
  // setup
  m_ui->setupUi(this);

  onTextChanged(m_ui->nameEdit->text());

  // connect
  connect(this, SIGNAL(accepted()), this, SLOT(onAccepted()));
  connect(m_ui->nameEdit, SIGNAL(textChanged(const QString&)), this, SLOT(onTextChanged(const QString&)));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when dialog was accepted. */
void AddConfigurationWindow::onAccepted()
{
  emit accepted(m_ui->nameEdit->text());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when text in edit changes. */
void AddConfigurationWindow::onTextChanged(const QString& text)
{
  QPushButton* button = m_ui->buttonBox->button(QDialogButtonBox::Ok);
  if (button)
  {
    button->setEnabled(!text.isEmpty());
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
