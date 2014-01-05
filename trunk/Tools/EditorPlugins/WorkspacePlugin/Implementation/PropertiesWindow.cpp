#include "PropertiesWindow.h"
#include "ui_PropertiesWindow.h"
#include <ObjectPool.h>
#include <FileSystemUtils.h>
#include <Settings.h>
#include <QDebug>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PropertiesWindow::PropertiesWindow(QWidget* parent) : QDockWidget(parent),
                                                      m_ui(new Ui_PropertiesWindow())
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
void PropertiesWindow::saveSettings()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void PropertiesWindow::loadSettings()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
