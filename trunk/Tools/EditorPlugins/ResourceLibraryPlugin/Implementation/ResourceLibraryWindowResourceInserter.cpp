#include "ResourceLibraryWindowResourceInserter.h"
#include "ResourceLibrary.h"
#include "ResourceLibraryWindow.h"
#include "ResourceItemFactory.h"
#include "ResourceItemGroup.h"
#include "ResourceItemTexture.h"
#include "ResourceItemTextureAtlas.h"
#include "ResourceLibraryDataModel.h"
#include <MainWindow.h>
#include <ObjectPool.h>
#include <Configuration.h>
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
  Configuration* configuration = ObjectPool::Instance()->getObject<Configuration>();
  Q_ASSERT(NULL != configuration);

  ResourceItem* newItem = m_factory->createItem(ResourceItemGroup::TypeName(), generateGroupName(), configuration->current());
  if (NULL != newItem)
  {
    m_library->insertItem(newItem);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceLibraryWindowResourceInserter::onAddTexture2D()
{
  Configuration* configuration = ObjectPool::Instance()->getObject<Configuration>();
  Q_ASSERT(NULL != configuration);

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

      ResourceItemTexture* newItem = static_cast<ResourceItemTexture*>(m_factory->createItem(ResourceItemTexture::TypeName(), name, configuration->current()));
      Q_ASSERT(NULL != newItem);

      if (NULL != newItem)
      {
        // set texture type
        newItem->setType(ETexture2D);

        // set path
        newItem->setFullPath(item);

        // add to model
        m_library->insertItem(newItem);
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void ResourceLibraryWindowResourceInserter::onAddTextureAtlas()
{
  Configuration* configuration = ObjectPool::Instance()->getObject<Configuration>();
  Q_ASSERT(NULL != configuration);

  // open file selection dialog
  QString directory = QFileDialog::getExistingDirectory(m_window, tr("Choose location for texture atlas image"));
  if ( ! directory.isEmpty())
  {
    ResourceItemTextureAtlas* newItem = static_cast<ResourceItemTextureAtlas*>(m_factory->createItem(ResourceItemTextureAtlas::TypeName(), tr("Texture"),
                                                                                                     configuration->current()));
    Q_ASSERT(NULL != newItem);

    if (NULL != newItem)
    {
      // set path
      newItem->setFullPath(directory);

      // add to model
      m_library->insertItem(newItem);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QString ResourceLibraryWindowResourceInserter::generateGroupName() const
{
  QString name;

  // get all present groups
  QList<ResourceItem*> availableGroups = m_library->items(ResourceItemGroup::TypeName());

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
