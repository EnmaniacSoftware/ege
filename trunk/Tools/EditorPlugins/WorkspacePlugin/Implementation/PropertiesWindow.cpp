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

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
using NPropertyObject::PropertyDefinition;
using NPropertyObject::PropertyValueContainer;
using NPropertyObject::PropertyObject;
using NPropertyObject::PropertyType;
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
  QtProperty* property = createProperty(definition.type(), definition.name(), definition.values(), definition.defaultValue(), definition.isReadOnly());

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
QtProperty* PropertiesWindow::createProperty(const PropertyType& type, const QString& name, const PropertyValueContainer& values, int defaultValue,
                                             bool readOnly) const
{
  QtProperty* property = NULL;

  QVariantList valuesForCurrentIndex;

  // create according to type
  switch (type)
  {
    case NPropertyObject::EGroup:

      property = m_groupManager->addProperty(name);
      break;

    case NPropertyObject::EBool:

      valuesForCurrentIndex = values.values(0);
      Q_ASSERT(1 == valuesForCurrentIndex.size());
      Q_ASSERT(valuesForCurrentIndex.at(0).canConvert<bool>());

      property = m_boolManager->addProperty(name);
      m_boolManager->setValue(property, valuesForCurrentIndex.at(0).toBool());
      break;

    case NPropertyObject::EInt:

      valuesForCurrentIndex = values.values(0);
      Q_ASSERT(1 == valuesForCurrentIndex.size());
      Q_ASSERT(valuesForCurrentIndex.at(0).canConvert<int>());

      property = m_intManager->addProperty(name);
      m_intManager->setValue(property, valuesForCurrentIndex.at(0).toInt());
      break;

    case NPropertyObject::ERect:

      valuesForCurrentIndex = values.values(0);
      Q_ASSERT(1 == valuesForCurrentIndex.size());
      Q_ASSERT(valuesForCurrentIndex.at(0).canConvert<QRect>());

      property = m_rectManager->addProperty(name);
      m_rectManager->setValue(property, valuesForCurrentIndex.at(0).toRect());
      break;

    case NPropertyObject::ESize:

      valuesForCurrentIndex = values.values(0);
      Q_ASSERT(1 == valuesForCurrentIndex.size());
      Q_ASSERT(valuesForCurrentIndex.at(0).canConvert<QSize>());

      property = m_sizeManager->addProperty(name);
      m_sizeManager->setValue(property, valuesForCurrentIndex.at(0).toSize());
      break;

    case NPropertyObject::EString:

      valuesForCurrentIndex = values.values(0);
      Q_ASSERT(1 == valuesForCurrentIndex.size());
      Q_ASSERT(valuesForCurrentIndex.at(0).canConvert<QString>());

      property = m_stringManager->addProperty(name);
      m_stringManager->setValue(property, valuesForCurrentIndex.at(0).toString());
      break;

    case NPropertyObject::EEnum:
    {
      // decompose values into names (strings) ans icons
      QMap<int, QIcon> enumIcons;
      QStringList enumNames;
      for (int i = 0; values.contains(i); ++i)
      {
        // get all values for a current index
        valuesForCurrentIndex = values.values(i);

        // go thru all values and determine acceptable values
        foreach (const QVariant& value, valuesForCurrentIndex)
        {
          // check if value is a string
          if (value.canConvert<QString>())
          {
            enumNames << value.toString();
          }
          else if (value.canConvert<QIcon>())
          {
            enumIcons[i] = value.value<QIcon>();
          }
        }
      }

      property = m_enumManager->addProperty(name);
      m_enumManager->setEnumNames(property, enumNames);
      m_enumManager->setEnumIcons(property, enumIcons);
      m_enumManager->setValue(property, defaultValue);
    }
      break;

    default:

      Q_ASSERT(false);
      break;
  }

  // set editability
  property->setEditable( ! readOnly);

  return property;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PropertiesWindow::onPropertyChanged(QtProperty* property)
{
  Q_ASSERT(NULL != m_propertyObject);

  // retrieve changed value
  QVariant value;
  if (property->propertyManager() == m_boolManager)
  {
    value = m_boolManager->value(property);
  }
  else if (property->propertyManager() == m_enumManager)
  {
    value = m_enumManager->value(property);
  }
  else if (property->propertyManager() == m_intManager)
  {
    value = m_intManager->value(property);
  }
  else if (property->propertyManager() == m_rectManager)
  {
    value = m_rectManager->value(property);
  }
  else if (property->propertyManager() == m_sizeManager)
  {
    value = m_sizeManager->value(property);
  }
  else if (property->propertyManager() == m_stringManager)
  {
    value = m_stringManager->value(property);
  }

  qDebug() << Q_FUNC_INFO << property->propertyName() << value;

  // propagate change to object
  m_propertyObject->update(property->propertyName(), value);

  // notify
  emit objectChanged(m_propertyObject);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
