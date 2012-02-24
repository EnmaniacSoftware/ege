#include "ResourceLibrary.h"
#include "ui_resourcelibrary.h"
#include "Config.h"
#include "MainWindow.h"
#include "ResourceLibraryDataModel.h"
#include "ResourceLibraryItemDelegate.h"
#include "ResourceItem.h"
#include <QMenu>
#include <QFile>
#include <QFileDialog>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceLibraryWindow::ResourceLibraryWindow(QWidget* parent) : QDockWidget(parent),
                                                                m_ui(new Ui_ResourceLibrary()),
                                                                m_model(new ResourceLibraryDataModel(this))
{
  // setup UI
  m_ui->setupUi(this);

  // create item delegate
  m_itemDelegate = new ResourceLibraryItemDelegate(m_ui->view);

  // set view delegate
  m_ui->view->setItemDelegate(m_itemDelegate);
  
  // set view model
  m_ui->view->setModel(m_model);

  connect(parent, SIGNAL(projectCreated()), this, SLOT(onProjectCreated()));
	connect(parent, SIGNAL(projectClosed()), this, SLOT(onProjectClosed()));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceLibraryWindow::~ResourceLibraryWindow()
{
  if (m_ui)
  {
    delete m_ui;
    m_ui = NULL;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Saves settings. */
void ResourceLibraryWindow::saveSettings(Config* config)
{
  Q_ASSERT(config);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Loads settings. */
void ResourceLibraryWindow::loadSettings(Config* config)
{
  Q_ASSERT(config);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when context menu is requested. */
void ResourceLibraryWindow::onQueueContextMenuRequested(const QPoint& pos)
{
  QMenu menu(this);

  QAction* action = NULL;

  QModelIndexList indexList = m_ui->view->selectionModel()->selectedIndexes();

  if (0 == indexList.size())
  {
    action = menu.addAction(tr("Add container"), this, SLOT(addContainer()));
  }
  else if (1 == indexList.size())
  {
    QModelIndex modelIndex = indexList.front();
    ResourceItem* item = static_cast<ResourceItem*>(modelIndex.internalPointer());

    if (ResourceItem::TYPE_CONTAINER == item->type())
    {
      action = menu.addAction(tr("Add container"), this, SLOT(addContainer()));
      action = menu.addAction(tr("Add resource"), this, SLOT(addResource()));
      action = menu.addAction(tr("Remove"), this, SLOT(removeItem()));
    }
  }
  else
  {
    action = menu.addAction(tr("Remove"), this, SLOT(removeItem()));
  }

	if (!menu.isEmpty())
  {
		menu.exec(m_ui->view->mapToGlobal(pos));
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when new project has been created/opened. */
void ResourceLibraryWindow::onProjectCreated()
{
  // establish connections
	connect(m_ui->stackedWidget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(onQueueContextMenuRequested(const QPoint&)));

  // show resources page
  m_ui->stackedWidget->setCurrentIndex(1);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when project has been closed. */
void ResourceLibraryWindow::onProjectClosed()
{
  // clean up model
  m_model->clear();

  // make disconnections
	disconnect(m_ui->stackedWidget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(onQueueContextMenuRequested(const QPoint&)));

  // hide resources page
  m_ui->stackedWidget->setCurrentIndex(0);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when container is requested to be added. */
void ResourceLibraryWindow::addContainer()
{
  ResourceItem* item;

  // get current seclection index
  QModelIndexList list = m_ui->view->selectionModel()->selectedIndexes();
  QModelIndex index = !list.isEmpty() ? list.front() : QModelIndex();

  // insert row at the begining of index
  if (!m_model->insertRow(0, index))
  {
    // error!
    return;
  }

  // update all columns
  for (int column = 0; column < m_model->columnCount(index); ++column)
  {
    QModelIndex child = m_model->index(0, column, index);

    m_model->setData(child, QVariant(tr("No name")), Qt::DisplayRole);
    m_model->setData(child, QVariant(ResourceItem::TYPE_CONTAINER), ResourceLibraryDataModel::TypeRole);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when resource is requested to be added. */
void ResourceLibraryWindow::addResource()
{
  QModelIndex index = m_ui->view->selectionModel()->selectedIndexes().first();

  // prepare filters
	QString filters = tr("Images");
	filters += QLatin1String(" (*.png *.jpg)");

  // open file selection dialog
	QStringList list = QFileDialog::getOpenFileNames(this, tr("Add resource"), QString(), filters);
  if (!list.isEmpty())
  {
    // insert rows at the begining of index
    if (!m_model->insertRows(0, list.size(), index))
    {
      // error!
      return;
    }

    // go thru all items
    for (int i = 0; i < list.size(); ++i)
    {
      // update all columns
      for (int column = 0; column < m_model->columnCount(index); ++column)
      {
        QModelIndex child = m_model->index(i, column, index);

        QString item = QDir::fromNativeSeparators(list[i]);
        QString name = item.section("/", -1);
        QString path = item.section("/", 0, -2);

        m_model->setData(child, QVariant(name), Qt::DisplayRole);
        m_model->setData(child, QVariant(path), ResourceLibraryDataModel::PathRole);
        m_model->setData(child, QVariant(ResourceItem::TYPE_IMAGE), ResourceLibraryDataModel::TypeRole);
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when resource item is requested to be removed. */
void ResourceLibraryWindow::removeItem()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! ISerializer override. Serializes into given buffer. */
QString ResourceLibraryWindow::serialize() const
{
  QString data;

  return data;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! ISerializer override. Unserializes from given data buffer. */
bool ResourceLibraryWindow::unserialize(const QString& data)
{
  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
