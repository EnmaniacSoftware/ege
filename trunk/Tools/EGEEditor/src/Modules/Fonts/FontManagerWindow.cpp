#include "Modules/Fonts/FontManagerWindow.h"
#include "Modules/Fonts/ResourceItemFont.h"
#include "ui_fontmanager.h"
#include "Config.h"
#include "MainWindow.h"
#include "Projects/Project.h"
#include "Resources/ResourceItem.h"
#include "Resources/ResourceItemImage.h"
#include "Resources/IResourceLibraryDataModel.h"
#include "Resources/ResourceItemFactory.h"
#include <QMenu>
#include <QFile>
#include <QFileDialog>
#include <QDebug>
#include <QWindowStateChangeEvent>
#include <QMdiSubwindow>
#include <QImageReader>
#include <QDir>
#include <QMenuBar>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
FontManagerWindow::FontManagerWindow(IResourceLibraryDataModel* resourceDataModel) : QDialog(NULL),
                                                                                     m_ui(new Ui_FontManager())/*,
                                                                m_model(new ResourceLibraryDataModel(this))*/
{
  // setup UI
  m_ui->setupUi(this);
  
  // connect to library data model
  connect(resourceDataModel->object(), SIGNAL(onItemAdded(ResourceItem*)), this, SLOT(onResourceModelItemAdded(ResourceItem*)));
  connect(resourceDataModel->object(), SIGNAL(onItemRemoved(ResourceItem*)), this, SLOT(onResourceModelItemRemoved(ResourceItem*)));

  // connect to widgets
  connect(m_ui->bitmaps, SIGNAL(currentIndexChanged(int)), this, SLOT(onFontBitmapChanged(int)));

  // retrieve current resource items
  QList<ResourceItem*> list = resourceDataModel->allItems();
  foreach (ResourceItem* item, list)
  {
    onResourceModelItemAdded(item);
  }

  // update title
  updateTitle();

  // register font resource item
  app->resourceItemFactory()->registerItem("font", ResourceItemFont::Create);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
FontManagerWindow::~FontManagerWindow()
{
  if (m_ui)
  {
    delete m_ui;
    m_ui = NULL;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Saves settings. */
void FontManagerWindow::saveSettings(Config* config)
{
  Q_ASSERT(config);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Loads settings. */
void FontManagerWindow::loadSettings(Config* config)
{
  Q_ASSERT(config);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when context menu is requested. */
void FontManagerWindow::onQueueContextMenuRequested(const QPoint& pos)
{
  QMenu menu(this);
  /*
  QAction* action = NULL;

  QModelIndexList indexList = m_ui->view->selectionModel()->selectedIndexes();

  if (0 == indexList.size())
  {
    action = menu.addAction(tr("Add container"), this, SLOT(onAddContainer()));
  }
  else if (1 == indexList.size())
  {
    QModelIndex modelIndex = indexList.front();
    ResourceItem* item = static_cast<ResourceItem*>(modelIndex.internalPointer());

    if ("container" == item->type())
    {
      action = menu.addAction(tr("Add container"), this, SLOT(onAddContainer()));
      action = menu.addAction(tr("Add resource"), this, SLOT(onAddResource()));
      action = menu.addAction(tr("Remove"), this, SLOT(onRemoveItems()));
    }
  }
  else
  {
    action = menu.addAction(tr("Remove"), this, SLOT(onRemoveItems()));
  }

	if (!menu.isEmpty())
  {
		menu.exec(m_ui->view->mapToGlobal(pos));
  }*/
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when new project has been created/opened. */
void FontManagerWindow::onProjectCreated(Project* project)
{
  // set view delegate
/*  ResourceLibraryItemDelegate* delegate = mainWindow()->project()->resourceLibraryItemDelegate();
  delegate->setView(m_ui->view);

  m_ui->view->setItemDelegate(delegate);

  // establish connections
	connect(m_ui->stackedWidget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(onQueueContextMenuRequested(const QPoint&)));
  connect(m_model, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), project, SLOT(onProjectDataChanged()));
  connect(m_model, SIGNAL(rowsInserted(const QModelIndex&, int, int)), project, SLOT(onProjectDataChanged()));
  connect(m_model, SIGNAL(rowsMoved(const QModelIndex&, int, int, const QModelIndex&, int)), project, SLOT(onProjectDataChanged()));
  connect(m_model, SIGNAL(rowsRemoved(const QModelIndex&, int, int)), project, SLOT(onProjectDataChanged()));

  // show resources page
  m_ui->stackedWidget->setCurrentIndex(1);*/
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when project has been closed. */
void FontManagerWindow::onProjectClosed()
{
  // reset view delegate
 // m_ui->view->setItemDelegate(NULL);

 // // clean up model
 // m_model->clear();

 // // make disconnections
	//disconnect(m_ui->stackedWidget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(onQueueContextMenuRequested(const QPoint&)));

 // // hide resources page
 // m_ui->stackedWidget->setCurrentIndex(0);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when container is requested to be added. */
void FontManagerWindow::onAddContainer()
{
  //ResourceItem* item;

  //// get current seclection index
  //QModelIndexList list = m_ui->view->selectionModel()->selectedIndexes();
  //QModelIndex index = !list.isEmpty() ? list.front() : QModelIndex();

  //ResourceItem* newItem = app->resourceItemFactory()->createItem("container", tr("No name"));
  //if (newItem)
  //{
  //  m_model->insertItem(index, newItem);
  //}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when resource is requested to be added. */
void FontManagerWindow::onAddResource()
{
 // QModelIndex index = m_ui->view->selectionModel()->selectedIndexes().first();

 // // prepare filters
	//QString filters = tr("Images");
	//filters += QLatin1String(" (*.png *.jpg)");

 // // open file selection dialog
	//QStringList list = QFileDialog::getOpenFileNames(this, tr("Add resource"), QString(), filters);
 // if (!list.isEmpty())
 // {
 //   // go thru all items
 //   for (int i = 0; i < list.size(); ++i)
 //   {
 //     QString item = QDir::fromNativeSeparators(list[i]);
 //     QString name = item.section("/", -1);
 //     QString path = item.section("/", 0, -2);

 //     ResourceItem* newItem = app->resourceItemFactory()->createItem("image", name);
 //     if (newItem)
 //     {
 //       QModelIndex childIndex = m_model->insertItem(index, newItem);

 //       m_model->setData(childIndex, QVariant(path), ResourceLibraryDataModel::PathRole);
 //     }
 //   }
 // }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when resource item is requested to be removed. */
void FontManagerWindow::onRemoveItems()
{
  //QModelIndexList indexList = m_ui->view->selectionModel()->selectedIndexes();
  //foreach (const QModelIndex& index, indexList)
  //{
  //  m_model->removeItem(index);
  //}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! ISerializer override. Serializes into given stream. */
bool FontManagerWindow::serialize(QXmlStreamWriter& stream) const
{
  stream.writeStartElement("resources");
  
  // serialize model
  //bool result = m_model->serialize(stream);

  stream.writeEndElement();

  return true;//result && !stream.hasError();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! ISerializer override. Unserializes from given data stream. */
bool FontManagerWindow::unserialize(QXmlStreamReader& stream)
{
  // unserialize model
  //bool result = m_model->unserialize(stream);
 
  return true;//result && !stream.hasError();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! QDialog override. Receives events to an object and should return true if the event was recognized and processed. */
bool FontManagerWindow::event(QEvent* event)
{
  switch (event->type())
  {
    case QEvent::Close:

      // ignore closing
      event->ignore();

      // update action menu
      setActionMenu(false);

      // just hide parent MdiSubWindow
      qobject_cast<QMdiSubWindow*>(parent())->hide();
      return true;

    case QEvent::WindowActivate:
    case QEvent::Show:

      event->accept();
      activated();
      return true;
  }

  return QDialog::event(event);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when item has been added to resource model. */
void FontManagerWindow::onResourceModelItemAdded(ResourceItem* item)
{
  if ("image" == item->type())
  {
    m_ui->bitmaps->addItem(item->name(), QVariant::fromValue(reinterpret_cast<void*>(item)));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when item has been removed from resource model. */
void FontManagerWindow::onResourceModelItemRemoved(ResourceItem* item)
{
  int index = m_ui->bitmaps->findData(QVariant::fromValue(reinterpret_cast<void*>(item)));
  
  if (-1 != index)
  {
    m_ui->bitmaps->removeItem(index);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when bitmap font selection changes. */
void FontManagerWindow::onFontBitmapChanged(int index)
{
  ResourceItemImage* item = reinterpret_cast<ResourceItemImage*>(m_ui->bitmaps->itemData(index).value<void*>());

  QImageReader imageReader(item->path() + QDir::separator() + item->name());
  imageReader.read(&m_image);

  m_ui->bitmapPreview->setPixmap(QPixmap::fromImage(m_image));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Window has been activated. */
void FontManagerWindow::activated()
{
  // enable action menu
  setActionMenu(true);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets action menu. */
void FontManagerWindow::setActionMenu(bool enable)
{
  QMenu* menu = app->menuBar()->findChild<QMenu*>("actionsMenu");
  menu->clear();

  menu->setEnabled(enable);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when title should be updated. */
void FontManagerWindow::updateTitle()
{
  QString title = tr("Font Manager");
  setWindowTitle(title);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
