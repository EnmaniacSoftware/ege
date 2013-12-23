#include "Configuration.h"
#include "MainWindow.h"
#include "AddConfigurationWindow.h"
#include "ui_configuration.h"
#include <Projects/Project.h>
#include <ObjectPool.h>
#include <QStatusBar>
#include <QMessageBox>
#include <QDebug>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const QString KConfigurationTag = "Configuration";
static const QString KNameAttribute    = "name";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Configuration::Configuration(QWidget* parent) : QWidget(parent),
                                                m_ui(new Ui_Configuration())
{
  // setup UI
  m_ui->setupUi(this);
   
  // setup icons
  m_ui->add->setIcon(QIcon(":/core/configuration-add"));
  m_ui->remove->setIcon(QIcon(":/core/configuration-remove"));

  onSelectionChanged();

  // connect
  connect(m_ui->add, SIGNAL(clicked(bool)), this, SLOT(onAddClicked()));
  connect(m_ui->remove, SIGNAL(clicked(bool)), this, SLOT(onRemoveClicked()));
  connect(m_ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onSelectionChanged()));
  connect(ObjectPool::Instance(), SIGNAL(objectAdded(QObject*)), this, SLOT(onObjectAdded(QObject*)));
  connect(ObjectPool::Instance(), SIGNAL(objectRemoved(QObject*)), this, SLOT(onObjectRemoved(QObject*)));

  // initially disable all
  setEnabled(false);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QString Configuration::current() const
{
  return m_ui->comboBox->currentText();
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
  m_ui->comboBox->removeItem(m_ui->comboBox->currentIndex());

  // update UI
  updateUI();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Configuration::onSelectionChanged()
{
  // update UI
  updateUI();

  // check if anything selected
  if (0 <= m_ui->comboBox->currentIndex())
  {
    // emit
    emit changed(m_ui->comboBox->currentText());
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Configuration::onAddConfiguration(const QString& name)
{
  // check if such configuration exists
  if (-1 != m_ui->comboBox->findText(name))
  {
    // cannot add
    QMessageBox::warning(this, tr("Failed!"), tr("Could not add configuration. Such name already exists!"));
    return;
  }

  qDebug() << "Configuration added:" << name;

  // add to pool
  m_ui->comboBox->addItem(name);

  // update UI
  updateUI();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Configuration::onObjectAdded(QObject* object)
{
  // check if project added
  if (qobject_cast<Project*>(object))
  {
    // create default configuration
    createDefault();

    // enable
    setEnabled(true);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Configuration::onObjectRemoved(QObject* object)
{
  // check if project removed
  if (qobject_cast<Project*>(object))
  {
    // remove all configurations
    removeAll();

    // disable
    setEnabled(false);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Configuration::removeAll()
{
  // remove all configurations
  m_ui->comboBox->clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Configuration::createDefault()
{
  static const QString KDefaultConfigurationName = tr("Default");

  // add default configuration
  onAddConfiguration(KDefaultConfigurationName);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Configuration::onSaveData(QXmlStreamWriter& stream)
{
  // go thru all configurations
  const int count = m_ui->comboBox->count();
  for (int i = 0; i < count; ++i)
  {
    const QString name = m_ui->comboBox->currentText();

    stream.writeStartElement(KConfigurationTag);

    stream.writeAttribute(KNameAttribute, name);

    // finalize
    stream.writeEndElement();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Configuration::onLoadData(QXmlStreamReader& stream)
{
  // check if proper element
  if (KConfigurationTag == stream.name())
  {
    const QString name = stream.attributes().value(KNameAttribute).toString();

    // add new configuration
    onAddConfiguration(name);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Configuration::updateUI()
{
  // enable REMOVE button only if more than 1 item
  m_ui->remove->setEnabled(1 < m_ui->comboBox->count());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
