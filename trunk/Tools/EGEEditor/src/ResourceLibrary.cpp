#include "ResourceLibrary.h"
#include "ui_resourcelibrary.h"
#include "Config.h"
#include "MainWindow.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceLibrary::ResourceLibrary(QWidget* parent) : QDockWidget(parent),
                                                    m_ui(new Ui_ResourceLibrary())
{
  // setup UI
  m_ui->setupUi(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceLibrary::~ResourceLibrary()
{
  if (m_ui)
  {
    delete m_ui;
    m_ui = NULL;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Saves settings. */
void ResourceLibrary::saveSettings(Config* config)
{
  Q_ASSERT(config);

  config->beginGroup("resource-library");
  config->setValue("size", size());
  config->setValue("pos", pos());
  config->setValue("dock", mainWindow()->dockWidgetArea(this));
  config->endGroup();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Loads settings. */
void ResourceLibrary::loadSettings(Config* config)
{
  Q_ASSERT(config);

  config->beginGroup("resource-library");
  resize(config->value("size", size()).toSize());
  move(config->value("pos", pos()).toPoint());
  mainWindow()->addDockWidget(static_cast<Qt::DockWidgetArea>(config->value("dock", mainWindow()->dockWidgetArea(this)).toInt()), this);
  config->endGroup();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
