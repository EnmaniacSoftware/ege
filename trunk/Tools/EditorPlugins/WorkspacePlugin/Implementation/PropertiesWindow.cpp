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
                                                      m_groupManager(new QtGroupPropertyManager(this))
{
  // setup UI
  m_ui->setupUi(this);

  // connect
 // connect(ObjectPool::Instance(), SIGNAL(objectAdded(QObject*)), this, SLOT(onObjectAdded(QObject*)));
 // connect(ObjectPool::Instance(), SIGNAL(objectRemoved(QObject*)), this, SLOT(onObjectRemoved(QObject*)));

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

  // check if anything to be attached
  if (NULL != object)
  {
    // show object's properties
    populateFromDefinition(object->propertiesDefinition());
  }

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
