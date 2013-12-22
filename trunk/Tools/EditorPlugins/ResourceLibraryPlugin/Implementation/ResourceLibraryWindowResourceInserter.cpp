#include "ResourceLibraryWindowResourceInserter.h"
#include "ResourceLibrary.h"
#include "ResourceLibraryWindow.h"
#include "ResourceItemFactory.h"
#include "ResourceItemGroup.h"
#include "ResourceItemTexture.h"
#include "ResourceLibraryDataModel.h"
#include <ObjectPool.h>
#include <QFileDialog>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceLibraryWindowResourceInserter::ResourceLibraryWindowResourceInserter(ResourceLibrary* library, ResourceLibraryWindow* libraryWindow,
                                                                             ResourceItemFactory* factory)
  : m_library(library),
    m_window(libraryWindow),
    m_factory(factory)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceLibraryWindowResourceInserter::~ResourceLibraryWindowResourceInserter()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceLibraryWindowResourceInserter::onAddGroup()
{
  // get current seclection index
  QModelIndexList list = m_window->selectedIndexes();
  QModelIndex index = ! list.isEmpty() ? list.front() : QModelIndex();

  ResourceItem* newItem = m_factory->createItem(ResourceItemGroup::TypeName(), generateGroupName());
  if (NULL != newItem)
  {
    m_library->model()->insertItem(index, newItem);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceLibraryWindowResourceInserter::onAddTexture2D()
{
  QModelIndex index = m_window->selectedIndexes().first();

  // prepare filters
  QString filters = tr("Images");
  filters += QLatin1String(" (*.png *.jpg)");

  // open file selection dialog
  QStringList list = QFileDialog::getOpenFileNames(m_window, tr("Add texture image"), QString(), filters);
  if ( ! list.isEmpty())
  {
    // go thru all items
    for (int i = 0; i < list.size(); ++i)
    {
      QString item = QDir::fromNativeSeparators(list[i]);
      QString name = item.section("/", -1);
      QString path = item.section("/", 0, -2);

      ResourceItemTexture* newItem = static_cast<ResourceItemTexture*>(m_factory->createItem(ResourceItemTexture::TypeName(), name));
      Q_ASSERT(NULL != newItem);

      if (NULL != newItem)
      {
        // set texture type
        newItem->setType(ETexture2D);

        // add to model
        QModelIndex childIndex = m_library->model()->insertItem(index, newItem);
        m_library->model()->setData(childIndex, QVariant(path), ResourceLibraryDataModel::PathRole);
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QString ResourceLibraryWindowResourceInserter::generateGroupName() const
{
  Q_ASSERT(NULL != m_library->model());

  QString name;

  // get all present groups
  QList<ResourceItem*> availableGroups = m_library->model()->items(ResourceItemGroup::TypeName());

  int index = 1;
  while (true)
  {
    // generate temporary name
    name = tr("Group %1").arg(index++);

    // check if does not exist
    bool found = false;
    foreach (const ResourceItem* item, availableGroups)
    {
      // check if found
      if (item->name() == name)
      {
        // done
        found = true;
        break;
      }
    }

    // check if not found
    if ( ! found)
    {
      // we are done
      break;
    }
  }

  return name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
