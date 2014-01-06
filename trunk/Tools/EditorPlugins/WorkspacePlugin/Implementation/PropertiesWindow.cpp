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
  // clear any old content
  m_ui->properties->clear();

  // show object's properties
  populateFromDefinition(object->propertiesDefinition());

  // change view page
  m_ui->stackedWidget->setCurrentIndex(KStackedPageIndexPropertiesView);
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

}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
