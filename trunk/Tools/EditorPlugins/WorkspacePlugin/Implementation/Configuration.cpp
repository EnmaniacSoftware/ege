#include "Configuration.h"
#include "MainWindow.h"
#include "AddConfigurationWindow.h"
#include "ChangeConfigurationNameWindow.h"
#include "ui_configuration.h"
#include <Projects/Project.h>
#include <ObjectPool.h>
#include <QStatusBar>
#include <QMessageBox>
#include <QDebug>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const QString KConfigurationsTag = "Configurations";
static const QString KConfigurationTag  = "Configuration";
static const QString KNameAttribute     = "name";
static const QString KActiveAttribute   = "active";
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Configuration::Configuration(QWidget* parent) : QWidget(parent),
                                                m_ui(new Ui_Configuration())
{
  // setup UI
  m_ui->setupUi(this);
   
  onSelectionChanged();

  // connect
  connect(m_ui->add, SIGNAL(clicked(bool)), this, SLOT(onAddClicked()));
  connect(m_ui->remove, SIGNAL(clicked(bool)), this, SLOT(onRemoveClicked()));
  connect(m_ui->edit, SIGNAL(clicked(bool)), this, SLOT(onEditClicked()));
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

  // store name of current configuration
  const QString name = current();

  // remove
  m_ui->comboBox->removeItem(m_ui->comboBox->currentIndex());

  // notify
  emit removed(name);

  // update UI
  updateUI();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Configuration::onEditClicked()
{
  // build up list of disallowed name
  // NOTE: It is not allowed to change the name into already existing. It is allowed, however to set the same name as current one. Thus, we do not add
  //       current configuration name into the pool
  QStringList disallowedNames;
  for (int i = 0; i < m_ui->comboBox->count(); ++i)
  {
    if (m_ui->comboBox->currentIndex() != i)
    {
      disallowedNames.push_back(m_ui->comboBox->itemText(i));
    }
  }

  ChangeConfigurationNameWindow window(current(), disallowedNames, this);
  connect(&window, SIGNAL(accepted(const QString&)), this, SLOT(onConfigurationNameChanged(const QString&)));
  window.exec();
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

  // mark project dirty
  Project* project = ObjectPool::Instance()->getObject<Project>();
  Q_ASSERT(NULL != project);
  project->onProjectDataChanged();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Configuration::onObjectAdded(QObject* object)
{
  // check if project added
  if (qobject_cast<Project*>(object))
  {
    // create default configuration
    createDefault();

    // connect
    connect(this, SIGNAL(nameChanged(QString, QString)), object, SLOT(onProjectDataChanged()));
    connect(this, SIGNAL(removed(QString)), object, SLOT(onProjectDataChanged()));

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

    // disconnect
    disconnect(this, SIGNAL(nameChanged(QString,QString)), object, SLOT(onProjectDataChanged()));
    disconnect(this, SIGNAL(removed(QString)), object, SLOT(onProjectDataChanged()));

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
  // write configurations group
  stream.writeStartElement(KConfigurationsTag);

  // go thru all configurations
  const int count = m_ui->comboBox->count();
  for (int i = 0; i < count; ++i)
  {
    const QString name = m_ui->comboBox->itemText(i);

    stream.writeStartElement(KConfigurationTag);

    stream.writeAttribute(KNameAttribute, name);
    stream.writeAttribute(KActiveAttribute, (name == current()) ? "true" : "false");

    stream.writeEndElement();
  }

  // finalize
  stream.writeEndElement();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Configuration::onLoadData(QXmlStreamReader& stream)
{
  // check if proper element
  if (KConfigurationsTag == stream.name())
  {
    // delete all current configurations
    removeAll();

    // read up all configurations
    while ( ! stream.atEnd() && ! stream.hasError())
    {
      QXmlStreamReader::TokenType token = stream.readNext();
      switch (token)
      {
        case QXmlStreamReader::StartElement:

          // check if resource item tag
          if (KConfigurationTag == stream.name())
          {
            // get required data
            const QString name = stream.attributes().value(KNameAttribute).toString();
            const bool active  = stream.attributes().value(KActiveAttribute).compare("true", Qt::CaseInsensitive);

            // add to pool
            m_ui->comboBox->addItem(name);

            // check if configuration is active one
            if (active)
            {
              // make active
              m_ui->comboBox->setCurrentIndex(m_ui->comboBox->count() - 1);
            }
          }
          break;

        case QXmlStreamReader::EndElement:

          if (KConfigurationsTag == stream.name())
          {
            // done
            break;
          }
          break;
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Configuration::onConfigurationNameChanged(const QString& name)
{
  // check if anything changed
  if (name != current())
  {
    QString oldName = current();

    // update current name
    m_ui->comboBox->setItemText(m_ui->comboBox->currentIndex(), name);

    // notify
    emit nameChanged(oldName, name);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Configuration::updateUI()
{
  // enable REMOVE button only if more than 1 item
  m_ui->remove->setEnabled(1 < m_ui->comboBox->count());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
