#include "Configuration.h"
#include "ui_configuration.h"
#include "ObjectPool.h"
#include "MainWindow.h"
#include "AddConfigurationWindow.h"
#include <QStatusBar>
#include <QMessageBox>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Configuration::Configuration(QWidget* parent) : QWidget(parent),
                                                m_ui(new Ui_Configuration())
{
  m_ui->setupUi(this);
   
  // setup
  m_ui->add->setIcon(QIcon(":/configuration-add"));
  m_ui->remove->setIcon(QIcon(":/configuration-remove"));

  onSelectionChanged();

  // connect
  connect(ObjectPool::Instance(), SIGNAL(objectAdded(QObject*)), this, SLOT(onObjectAdded(QObject*)));
  connect(m_ui->add, SIGNAL(clicked(bool)), this, SLOT(onAddClicked()));
  connect(m_ui->remove, SIGNAL(clicked(bool)), this, SLOT(onRemoveClicked()));
  connect(m_ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onSelectionChanged()));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when object has been added into pool. */
void Configuration::onObjectAdded(QObject* object)
{
  // check if main window added
  MainWindow* mainWindow = qobject_cast<MainWindow*>(object);
  if (mainWindow)
  {
    // add self to status bar
    mainWindow->statusBar()->addPermanentWidget(this);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when add button has been clicked. */
void Configuration::onAddClicked()
{
  AddConfigurationWindow window;
  connect(&window, SIGNAL(accepted(const QString&)), this, SLOT(onAddConfiguration(const QString&)));
  window.exec();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when remove button has been clicked. */
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
  m_ui->comboBox->removeItem(m_ui->comboBox->currentIndex());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when onfiguration selection changed. */
void Configuration::onSelectionChanged()
{
  m_ui->remove->setEnabled(0 <= m_ui->comboBox->currentIndex());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when configuration with given name is to be added. */
void Configuration::onAddConfiguration(const QString& name)
{
  // check if such configuration exists
  if (-1 != m_ui->comboBox->findText(name))
  {
    // cannot add
    QMessageBox::warning(this, tr("Failed!"), tr("Could not add configuration. Such name already exists!"));
    return;
  }

  // add to pool
  m_ui->comboBox->addItem(name);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
