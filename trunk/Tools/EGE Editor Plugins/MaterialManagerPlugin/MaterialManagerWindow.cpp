#include "MaterialManagerWindow.h"
#include "ui_MaterialManagerWindow.h"
#include "ResourceItemMaterial.h"
#include <ResourceItemFactory.h>
#include <Core.h>
#include <MainWindow.h>
#include <CoreConstants.h>
#include <QMenu>
#include <QMenuBar>
#include <QFile>
#include <QFileDialog>
#include <QDebug>
#include <ObjectPool.h>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
MaterialManagerWindow::MaterialManagerWindow(QWidget* parent) : QDialog(parent),
                                                                m_ui(new Ui_MaterialManager())
{
  // setup UI
  m_ui->setupUi(this);
  
  // attach resource library swpecific data
  attachToResourceLibrary();

  // update menu
  updateMenus();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
MaterialManagerWindow::~MaterialManagerWindow()
{
  if (m_ui)
  {
    delete m_ui;
    m_ui = NULL;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Saves settings. */
void MaterialManagerWindow::saveSettings(Config* config)
{
  Q_ASSERT(config);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Loads settings. */
void MaterialManagerWindow::loadSettings(Config* config)
{
  Q_ASSERT(config);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! ISerializer override. Serializes into given stream. */
bool MaterialManagerWindow::serialize(QXmlStreamWriter& stream) const
{
  //stream.writeStartElement("resources");
  //
  //// serialize model
  //bool result = m_model->serialize(stream);

  //stream.writeEndElement();

  //return result && !stream.hasError();

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! ISerializer override. Unserializes from given data stream. */
bool MaterialManagerWindow::unserialize(QXmlStreamReader& stream)
{
  //// unserialize model
  //bool result = m_model->unserialize(stream);
 
  //return result && !stream.hasError();

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates menus. */
void MaterialManagerWindow::updateMenus()
{
  MainWindow* mainWindow = Core::instance()->mainWindow();
  Q_ASSERT(mainWindow);

  QMenu* menu = mainWindow->menuBar()->findChild<QMenu*>(MENU_MODULE);
  Q_ASSERT(menu);

  QAction* action = new QAction(tr("Material Manager"), menu);
  action->setShortcut(QKeySequence("Ctrl+Shift+M"));
  connect(action, SIGNAL(triggered()), this, SLOT(show()));
  menu->addAction(action);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Attaches resource library specific data. */
void MaterialManagerWindow::attachToResourceLibrary()
{
  MainWindow* mainWindow = Core::instance()->mainWindow();
  Q_ASSERT(mainWindow);
  
  ResourceItemFactory* resourceItemFactory = ObjectPool::instance()->getObject<ResourceItemFactory>();

  if (!resourceItemFactory->registerItem("material", ResourceItemMaterial::Create))
  {
    qWarning() << "Could not register ResourceItemMaterial";
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
