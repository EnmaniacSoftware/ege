#include "AddConfigurationWindow.h"
#include "ConfigurationNameValidator.h"
#include <QPushButton>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
AddConfigurationWindow::AddConfigurationWindow(QWidget* parent) : QDialog(parent)
{
  // setup
  setupUi(this);

  onTextChanged(nameEdit->text());

  // connect
  connect(this, SIGNAL(accepted()), this, SLOT(onAccepted()));
  connect(nameEdit, SIGNAL(textChanged(const QString&)), this, SLOT(onTextChanged(const QString&)));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AddConfigurationWindow::onAccepted()
{
  emit accepted(nameEdit->text());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void AddConfigurationWindow::onTextChanged(const QString& text)
{
  QPushButton* button = buttonBox->button(QDialogButtonBox::Ok);
  if (NULL != button)
  {
    // validate name
    QString validatedString = ConfigurationNameValidator::Validate(text);

    if ( ! text.isEmpty() && (validatedString == text))
    {
      button->setEnabled(true);
    }
    else
    {
      button->setEnabled(false);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
