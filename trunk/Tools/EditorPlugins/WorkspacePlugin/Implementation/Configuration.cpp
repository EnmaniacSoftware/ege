#include "Configuration.h"
#include "MainWindow.h"
#include "AddConfigurationWindow.h"
#include <Projects/Project.h>
#include <ObjectPool.h>
#include <QStatusBar>
#include <QMessageBox>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const QString KConfigurationTag = "Configuration";
static const QString KNameAttribute    = "name";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Configuration::Configuration(QWidget* parent) : QWidget(parent)
{
  // setup UI
  setupUi(this);
   
  // setup icons
  add->setIcon(QIcon(":/core/configuration-add"));
  remove->setIcon(QIcon(":/core/configuration-remove"));

  onSelectionChanged();

  // connect
  connect(add, SIGNAL(clicked(bool)), this, SLOT(onAddClicked()));
  connect(remove, SIGNAL(clicked(bool)), this, SLOT(onRemoveClicked()));
  connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onSelectionChanged()));
  connect(ObjectPool::Instance(), SIGNAL(objectAdded(QObject*)), this, SLOT(onObjectAdded(QObject*)));
  connect(ObjectPool::Instance(), SIGNAL(objectRemoved(QObject*)), this, SLOT(onObjectRemoved(QObject*)));

  // initially disable all
  setEnabled(false);
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

  // update UI
  updateUI();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Configuration::onSelectionChanged()
{
  // update UI
  updateUI();

  // check if anything selected
  if (0 <= comboBox->currentIndex())
  {
    // emit
    emit changed(comboBox->currentText());
  }
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
  comboBox->clear();
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
  const int count = comboBox->count();
  for (int i = 0; i < count; ++i)
  {
    const QString name = comboBox->currentText();

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
  remove->setEnabled(1 < comboBox->count());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
