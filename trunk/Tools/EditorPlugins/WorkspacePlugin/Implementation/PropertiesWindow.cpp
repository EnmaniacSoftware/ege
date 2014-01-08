#include "PropertiesWindow.h"
#include "ui_PropertiesWindow.h"
#include "PropertyObject.h"
#include <qttreepropertybrowser.h>
#include <qtpropertymanager.h>
#include <qteditorfactory.h>
#include <ObjectPool.h>
#include <FileSystemUtils.h>
#include <Settings.h>
#include <QDebug>

using namespace NPropertyObject;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
static const int KStackedPageIndexObjectNotAvailable = 0;
static const int KStackedPageIndexPropertiesView     = 1;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PropertiesWindow::PropertiesWindow(QWidget* parent) : QDockWidget(parent),
                                                      m_ui(new Ui_PropertiesWindow()),
                                                      m_boolManager(new QtBoolPropertyManager(this)),
                                                      m_intManager(new QtIntPropertyManager(this)),
                                                      m_stringManager(new QtStringPropertyManager(this)),
                                                      m_sizeManager(new QtSizePropertyManager(this)),
                                                      m_rectManager(new QtRectPropertyManager(this)),
                                                      m_enumManager(new QtEnumPropertyManager(this)),
                                                      m_groupManager(new QtGroupPropertyManager(this)),
                                                      m_propertyObject(NULL)
{
  // setup UI
  m_ui->setupUi(this);

  // create factories for editing
  QtCheckBoxFactory* checkBoxFactory = new QtCheckBoxFactory(this);
  QtSpinBoxFactory* spinBoxFactory = new QtSpinBoxFactory(this);
  QtLineEditFactory* lineEditFactory = new QtLineEditFactory(this);
  QtEnumEditorFactory* comboBoxFactory = new QtEnumEditorFactory(this);

  // attach factories to managers
  m_ui->properties->setFactoryForManager(m_boolManager, checkBoxFactory);
  m_ui->properties->setFactoryForManager(m_intManager, spinBoxFactory);
  m_ui->properties->setFactoryForManager(m_stringManager, lineEditFactory);
  m_ui->properties->setFactoryForManager(m_sizeManager->subIntPropertyManager(), spinBoxFactory);
  m_ui->properties->setFactoryForManager(m_rectManager->subIntPropertyManager(), spinBoxFactory);
  m_ui->properties->setFactoryForManager(m_enumManager, comboBoxFactory);

  // load settings
  loadSettings();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PropertiesWindow::~PropertiesWindow()
{
  // save settings
  saveSettings();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PropertiesWindow::attach(PropertyObject* object)
{
  // clear view
  // NOTE: view DOES NOT deallocate properties
  m_ui->properties->clear();

  // NOTE: managers are real owners of created properties
  m_boolManager->clear();
  m_intManager->clear();
  m_stringManager->clear();
  m_sizeManager->clear();
  m_rectManager->clear();
  m_enumManager->clear();
  m_groupManager->clear();

  // disconnect
  // NOTE: we dont want notification upon initial creation
  disconnect(m_boolManager, SIGNAL(propertyChanged(QtProperty*)), this, SLOT(onPropertyChanged(QtProperty*)));
  disconnect(m_intManager, SIGNAL(propertyChanged(QtProperty*)), this, SLOT(onPropertyChanged(QtProperty*)));
  disconnect(m_stringManager, SIGNAL(propertyChanged(QtProperty*)), this, SLOT(onPropertyChanged(QtProperty*)));
  disconnect(m_sizeManager, SIGNAL(propertyChanged(QtProperty*)), this, SLOT(onPropertyChanged(QtProperty*)));
  disconnect(m_rectManager, SIGNAL(propertyChanged(QtProperty*)), this, SLOT(onPropertyChanged(QtProperty*)));
  disconnect(m_enumManager, SIGNAL(propertyChanged(QtProperty*)), this, SLOT(onPropertyChanged(QtProperty*)));
  disconnect(m_groupManager, SIGNAL(propertyChanged(QtProperty*)), this, SLOT(onPropertyChanged(QtProperty*)));

  // check if anything to be attached
  if (NULL != object)
  {
    // show object's properties
    populateFromDefinition(object->propertiesDefinition());
  }

  // reconnect
  connect(m_boolManager, SIGNAL(propertyChanged(QtProperty*)), this, SLOT(onPropertyChanged(QtProperty*)));
  connect(m_intManager, SIGNAL(propertyChanged(QtProperty*)), this, SLOT(onPropertyChanged(QtProperty*)));
  connect(m_stringManager, SIGNAL(propertyChanged(QtProperty*)), this, SLOT(onPropertyChanged(QtProperty*)));
  connect(m_sizeManager, SIGNAL(propertyChanged(QtProperty*)), this, SLOT(onPropertyChanged(QtProperty*)));
  connect(m_rectManager, SIGNAL(propertyChanged(QtProperty*)), this, SLOT(onPropertyChanged(QtProperty*)));
  connect(m_enumManager, SIGNAL(propertyChanged(QtProperty*)), this, SLOT(onPropertyChanged(QtProperty*)));
  connect(m_groupManager, SIGNAL(propertyChanged(QtProperty*)), this, SLOT(onPropertyChanged(QtProperty*)));

  // store pointer to object for further use
  m_propertyObject = object;

  // change view page
  m_ui->stackedWidget->setCurrentIndex((NULL != object) ? KStackedPageIndexPropertiesView : KStackedPageIndexObjectNotAvailable);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PropertiesWindow::saveSettings()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PropertiesWindow::loadSettings()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PropertiesWindow::populateFromDefinition(const QList<PropertyDefinition>& properties)
{
  // go thru all definitions
  foreach (const PropertyDefinition& definition, properties)
  {
    // create new property from definition
    QtProperty* property = createAndPopulatePropertyFromDefinition(definition);

    // add to editor
    m_ui->properties->addProperty(property);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QtProperty* PropertiesWindow::createAndPopulatePropertyFromDefinition(const PropertyDefinition& definition) const
{
  // create property based on definition
  QtProperty* property = createProperty(definition.type(), definition.name(), definition.value());

  // create and add all subproperties based on child definitions
  foreach (const PropertyDefinition& childDefinition, definition.children())
  {
    QtProperty* childProperty = createAndPopulatePropertyFromDefinition(childDefinition);

    // add to base property
    property->addSubProperty(childProperty);
  }

  return property;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QtProperty* PropertiesWindow::createProperty(const NPropertyObject::PropertyType& type, const QString& name, const QString& value) const
{
  QtProperty* property = NULL;

  // create according to type
  switch (type)
  {
    case NPropertyObject::EGroup:

      property = m_groupManager->addProperty(name);
      break;

    case NPropertyObject::EBool:

      property = m_boolManager->addProperty(name);
      //m_boolManager->setValue(property);
      break;

    case NPropertyObject::EInt:

      property = m_intManager->addProperty(name);
      m_intManager->setValue(property, value.toInt());
      break;

    case NPropertyObject::ERect:

      property = m_rectManager->addProperty(name);
      //m_rectManager->setValue(property, value);
      break;

    case NPropertyObject::ESize:

      property = m_sizeManager->addProperty(name);
      //m_sizeManager->setValue(property, value);
      break;

    case NPropertyObject::EString:

      property = m_stringManager->addProperty(name);
      m_stringManager->setValue(property, value);
      break;

    default:

      Q_ASSERT(false);
      break;
  }

  return property;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PropertiesWindow::onPropertyChanged(QtProperty* property)
{
  Q_ASSERT(NULL != m_propertyObject);

  qDebug() << Q_FUNC_INFO << property->propertyName() << property->displayText();

  QVariant value;

  QString val = m_stringManager->value(property);
  QString vax = property->displayText();

//  if (m_groupManager->hasValue(property))
//  {
//    value = m_groupManager->valueText(property);
//  }

  value = property->displayText();

  // propagate
  m_propertyObject->update(property->propertyName(), value);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
