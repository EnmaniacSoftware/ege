#include "ChangeConfigurationNameWindow.h"
#include "ConfigurationNameValidator.h"
#include <QPushButton>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ChangeConfigurationNameWindow::ChangeConfigurationNameWindow(const QString& initialName, const QStringList& restrictedNames, QWidget* parent)
  : QDialog(parent),
    m_restrictedNames(restrictedNames)
{
  // setup
  setupUi(this);

  // connect
  connect(this, SIGNAL(accepted()), this, SLOT(onAccepted()));
  connect(lineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(onTextChanged(const QString&)));

  // set value being edited
  lineEdit->setText(initialName);
  lineEdit->selectAll();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ChangeConfigurationNameWindow::onAccepted()
{
  emit accepted(lineEdit->text());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ChangeConfigurationNameWindow::onTextChanged(const QString& text)
{
  QPushButton* button = buttonBox->button(QDialogButtonBox::Ok);
  if (NULL != button)
  {
    // check if name is allowed
    // Name is allowed when:
    // - non empty
    // - does not occur in the list of restricted names
    // - does not contain improper characters

    // validate name
    QString validatedString = ConfigurationNameValidator::Validate(text);

    if ( ! validatedString.isEmpty() && (validatedString == text) && ! m_restrictedNames.contains(text))
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
