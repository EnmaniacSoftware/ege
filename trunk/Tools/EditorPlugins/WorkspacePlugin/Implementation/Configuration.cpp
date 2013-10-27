#include "Configuration.h"
#include "MainWindow.h"
#include "AddConfigurationWindow.h"
#include <QStatusBar>
#include <QMessageBox>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Configuration::Configuration(QWidget* parent) : QWidget(parent)
{
  // setup UI
  setupUi(this);
   
  // setup
  add->setIcon(QIcon(":/configuration-add"));
  remove->setIcon(QIcon(":/configuration-remove"));

  onSelectionChanged();

  // connect
  connect(add, SIGNAL(clicked(bool)), this, SLOT(onAddClicked()));
  connect(remove, SIGNAL(clicked(bool)), this, SLOT(onRemoveClicked()));
  connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onSelectionChanged()));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Configuration::onAddClicked()
{
  AddConfigurationWindow window;
  connect(&window, SIGNAL(accepted(const QString&)), this, SLOT(onAddConfiguration(const QString&)));
  window.exec();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Configuration::onRemoveClicked()
{
  // prompt
  if (QMessageBox::No == QMessageBox::warning(this, tr("Remove configuration"), tr("Are you sure you want to remove current configuration ?"),
                                              QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
  {
    // abort
    return;
  }

  // remove
  comboBox->removeItem(comboBox->currentIndex());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Configuration::onSelectionChanged()
{
  remove->setEnabled(0 <= comboBox->currentIndex());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Configuration::onAddConfiguration(const QString& name)
{
  // check if such configuration exists
  if (-1 != comboBox->findText(name))
  {
    // cannot add
    QMessageBox::warning(this, tr("Failed!"), tr("Could not add configuration. Such name already exists!"));
    return;
  }

  // add to pool
  comboBox->addItem(name);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
