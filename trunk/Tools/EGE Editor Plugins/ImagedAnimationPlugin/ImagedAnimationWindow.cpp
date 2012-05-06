#include "ImagedAnimationWindow.h"
#include "ui_ImagedAnimationWindow.h"
#include <MainWindow.h>
#include <CoreConstants.h>
#include <ObjectPool.h>
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

  QStringList list;
  //list << "apple_lvl1_blink";
  //list << "apple_lvl1_idle";
  //list << "apple_lvl1_dance";
  //list << "apple_lvl1_chomp";
  //list << "apple_lvl2_blink";
  //list << "apple_lvl2_idle";
  //list << "apple_lvl2_dance";
  //list << "apple_lvl2_chomp";
  //list << "apple_lvl3_idle";
  //list << "apple_lvl3_dance";
  //list << "apple_lvl3_chomp";
  //list << "plum_lvl1_blink";
  //list << "plum_lvl1_idle";
  //list << "plum_lvl1_dance";
  //list << "plum_lvl1_chomp";
  //list << "plum_lvl2_blink";
  //list << "plum_lvl2_idle";
  //list << "plum_lvl2_dance";
  //list << "plum_lvl2_chomp";
  //list << "plum_lvl3_idle";
  //list << "plum_lvl3_dance";
  //list << "plum_lvl3_chomp";
  //list << "strawberry_lvl1_blink";
  //list << "strawberry_lvl1_idle";
  //list << "strawberry_lvl1_dance";
  //list << "strawberry_lvl1_chomp";
  //list << "strawberry_lvl2_blink";
  //list << "strawberry_lvl2_idle";
  //list << "strawberry_lvl2_dance";
  //list << "strawberry_lvl2_chomp";
  //list << "strawberry_lvl3_idle";
  //list << "strawberry_lvl3_dance";
  //list << "strawberry_lvl3_chomp";
  //list << "peach_lvl1_blink";
  //list << "peach_lvl1_idle";
  //list << "peach_lvl1_dance";
  //list << "peach_lvl1_chomp";
  //list << "peach_lvl2_blink";
  //list << "peach_lvl2_idle";
  //list << "peach_lvl2_dance";
  //list << "peach_lvl2_chomp";
  //list << "peach_lvl3_idle";
  //list << "peach_lvl3_dance";
  //list << "peach_lvl3_chomp";
  //list << "peach_lvl3_blink";
  //list << "sick_blink";
  //list << "sick_idle";
  //list << "sick_dance";
  //list << "sick_chomp";
  //list << "sick_chained";
  //list << "cherry_lvl1_blink";
  //list << "cherry_lvl1_idle";
  //list << "cherry_lvl1_dance";
  //list << "cherry_lvl1_chomp";
  //list << "cherry_lvl2_blink";
  //list << "cherry_lvl2_idle";
  //list << "cherry_lvl2_dance";
  //list << "cherry_lvl2_chomp";
  //list << "cherry_lvl3_idle";
  //list << "cherry_lvl3_dance";
  //list << "cherry_lvl3_blink";
  //list << "orange_lvl1_blink";
  //list << "orange_lvl1_idle";
  //list << "orange_lvl1_dance";
  //list << "orange_lvl1_chomp";
  //list << "orange_lvl2_blink";
  //list << "orange_lvl2_idle";
  //list << "orange_lvl2_dance";
  //list << "orange_lvl2_chomp";
  //list << "orange_lvl3_idle";
  //list << "orange_lvl3_dance";
  //list << "banana_lvl1_blink";
  //list << "banana_lvl1_idle";
  //list << "banana_lvl1_dance";
  //list << "banana_lvl1_chomp";
  //list << "banana_lvl2_blink";
  //list << "banana_lvl2_idle";
  //list << "banana_lvl2_dance";
  //list << "banana_lvl2_chomp";
  //list << "banana_lvl3_idle";
  //list << "banana_lvl3_chomp";
  //list << "banana_lvl3_blink";
  //list << "neutral_blink";
  //list << "neutral_idle";
  //list << "neutral_dance";
  //list << "neutral_chomp";

  list << "003_translate_scale_rotate_banana";

  float scales[] = { 1.0f };
//  float scales[] = { 0.25f, 0.5f };

  for (int i = 0; i < sizeof (scales) / sizeof (scales[0]); ++i)
  {
    foreach (const QString& string, list)
    {
      // TAGE
      QFile file(QString("data/") + string + QString(".xml"));
      file.open(QIODevice::Text | QIODevice::ReadOnly);
      QXmlStreamReader input(&file);

      QString outXml;
      QXmlStreamWriter output(&outXml);
      output.setAutoFormatting(true);

      m_converter.convert(input, output, string, scales[i]);

      QFile fileOut(QString("ege_%1_%2.xml").arg(string).arg(QString::number(scales[i]).remove(",")));
      fileOut.open(QIODevice::Text | QIODevice::WriteOnly);
      QTextStream out(&fileOut);
      out << outXml;
    }
  }
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
  MainWindow* mainWindow = ObjectPool::Instance()->getObject<MainWindow>();
  if (NULL == mainWindow)
  {
    // error!
    qWarning() << Q_FUNC_INFO << "No MainWindow found!";
    return;
  }

  QMenu* menu = mainWindow->menuBar()->findChild<QMenu*>(MENU_MODULE);
  Q_ASSERT(menu);

  QAction* action = new QAction(tr("Imaged Animation"), menu);
  action->setShortcut(QKeySequence("Ctrl+Shift+A"));
  connect(action, SIGNAL(triggered()), this, SLOT(show()));
  menu->addAction(action);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
