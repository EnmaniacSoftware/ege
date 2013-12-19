#include "ResourceLibraryWindowGroupAdder.h"
#include "ResourceLibrary.h"
#include "ResourceLibraryWindow.h"
#include "ResourceItemFactory.h"
#include "ResourceItemContainer.h"
#include "ResourceLibraryDataModel.h"
#include <ObjectPool.h>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceLibraryWindowGroupAdder::ResourceLibraryWindowGroupAdder()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceLibraryWindowGroupAdder::~ResourceLibraryWindowGroupAdder()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceLibraryWindowGroupAdder::onAdd()
{
  ResourceLibrary* library = ObjectPool::Instance()->getObject<ResourceLibrary>();
  Q_ASSERT(NULL != library);

  ResourceLibraryWindow* window = ObjectPool::Instance()->getObject<ResourceLibraryWindow>();
  Q_ASSERT(NULL != window);

  ResourceItemFactory* factory = ObjectPool::Instance()->getObject<ResourceItemFactory>();
  Q_ASSERT(NULL != factory);

  // get current seclection index
  QModelIndexList list = window->selectedIndexes();
  QModelIndex index = ! list.isEmpty() ? list.front() : QModelIndex();

  if (NULL != factory)
  {
    ResourceItem* newItem = factory->createItem(ResourceItemContainer::TypeName(), generateGroupName());
    if (NULL != newItem)
    {
      library->model()->insertItem(index, newItem);

      // show resources view
      //m_ui->stackedWidget->setCurrentIndex(KStackedPageIndexResourcesView);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QString ResourceLibraryWindowGroupAdder::generateGroupName() const
{
  ResourceLibrary* library = ObjectPool::Instance()->getObject<ResourceLibrary>();
  Q_ASSERT(NULL != library);
  Q_ASSERT(NULL != library->model());

  QString name;

  // get all present groups
  QList<ResourceItem*> availableGroups = library->model()->items(ResourceItemContainer::TypeName());

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
