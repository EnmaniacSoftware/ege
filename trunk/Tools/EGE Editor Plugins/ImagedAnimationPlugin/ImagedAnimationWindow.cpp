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

  // TAGE
  QFile file("data/chomp_1_banana.xml");
  file.open(QIODevice::Text | QIODevice::ReadOnly);
  QXmlStreamReader input(&file);

  QString string;
  QXmlStreamWriter output(&string);
  output.setAutoFormatting(true);

  m_converter.convert(input, output);

  QFile fileOut("ege_chomp_1_banana.xml");
  bool a= fileOut.open(QIODevice::Text | QIODevice::WriteOnly);
  QTextStream out(&fileOut);
  out << string;
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
  MainWindow* mainWindow = Core::Instance()->mainWindow();

  QMenu* menu = mainWindow->menuBar()->findChild<QMenu*>(MENU_MODULE);
  Q_ASSERT(menu);

  QAction* action = new QAction(tr("Imaged Animation"), menu);
  action->setShortcut(QKeySequence("Ctrl+Shift+A"));
  connect(action, SIGNAL(triggered()), this, SLOT(show()));
  menu->addAction(action);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
