#include "ResourceLibrary.h"
#include "ui_resourcelibrary.h"
#include "Config.h"
#include "MainWindow.h"
#include "ResourceLibraryDataModel.h"
#include "ResourceLibraryItemDelegate.h"
#include "ResourceLibraryItem.h"
#include <QMenu>
#include <QFile>
#include <QFileDialog>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceLibrary::ResourceLibrary(QWidget* parent) : QDockWidget(parent),
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

  // establish connections
	connect(m_ui->stackedWidget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(onQueueContextMenuRequested(const QPoint&)));

  connect(parent, SIGNAL(projectCreated()), this, SLOT(onProjectCreated()));
	connect(parent, SIGNAL(projectClosed()), this, SLOT(onProjectClosed()));
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
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Loads settings. */
void ResourceLibrary::loadSettings(Config* config)
{
  Q_ASSERT(config);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when context menu is requested. */
void ResourceLibrary::onQueueContextMenuRequested(const QPoint& pos)
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
    ResourceLibraryItem* item = static_cast<ResourceLibraryItem*>(modelIndex.internalPointer());

    if (ResourceLibraryItem::TYPE_CONTAINER == item->type())
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
void ResourceLibrary::onProjectCreated()
{
  // load up data if project file exists
  if (QFile::exists(mainWindow()->project()->fullPath()))
  {
    // load resource library
    if (!m_model->load(mainWindow()->project()->fullPath()))
    {
      // TAGE - error
    }
  }
  else
  {
    // create empty
    m_model->createDefault();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when project has been closed. */
void ResourceLibrary::onProjectClosed()
{
  m_model->clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when container is requested to be added. */
void ResourceLibrary::addContainer()
{
  ResourceLibraryItem* item;

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
    m_model->setData(child, QVariant(ResourceLibraryItem::TYPE_CONTAINER), ResourceLibraryDataModel::TypeRole);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when resource is requested to be added. */
void ResourceLibrary::addResource()
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
        m_model->setData(child, QVariant(ResourceLibraryItem::TYPE_IMAGE), ResourceLibraryDataModel::TypeRole);
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when resource item is requested to be removed. */
void ResourceLibrary::removeItem()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
