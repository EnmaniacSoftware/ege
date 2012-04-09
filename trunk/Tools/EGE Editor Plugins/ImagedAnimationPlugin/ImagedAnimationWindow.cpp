#include "ImagedAnimationWindow.h"
#include "ui_ImagedAnimationWindow.h"
#include "MainWindow.h"
#include <Core.h>
#include <MainWindow.h>
#include <CoreConstants.h>
#include <QMenu>
#include <QMenuBar>
#include <QFile>
#include <QFileDialog>
#include <QDebug>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ImagedAnimationWindow::ImagedAnimationWindow(QWidget* parent) : QDialog(parent),
                                                                m_ui(new Ui_ImagedAnimation())
{
  // setup UI
  m_ui->setupUi(this);
  
  updateMenus();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ImagedAnimationWindow::~ImagedAnimationWindow()
{
  if (m_ui)
  {
    delete m_ui;
    m_ui = NULL;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Saves settings. */
void ImagedAnimationWindow::saveSettings(Config* config)
{
  Q_ASSERT(config);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Loads settings. */
void ImagedAnimationWindow::loadSettings(Config* config)
{
  Q_ASSERT(config);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! ISerializer override. Serializes into given stream. */
bool ImagedAnimationWindow::serialize(QXmlStreamWriter& stream) const
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
bool ImagedAnimationWindow::unserialize(QXmlStreamReader& stream)
{
  //// unserialize model
  //bool result = m_model->unserialize(stream);
 
  //return result && !stream.hasError();

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates menus. */
void ImagedAnimationWindow::updateMenus()
{
  MainWindow* mainWindow = Core::instance()->mainWindow();

  QMenu* menu = mainWindow->menuBar()->findChild<QMenu*>(MENU_MODULE);
  Q_ASSERT(menu);

  QAction* action = new QAction(tr("Imaged Animation"), menu);
  action->setShortcut(QKeySequence("Ctrl+Shift+A"));
  connect(action, SIGNAL(triggered()), this, SLOT(show()));
  menu->addAction(action);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
