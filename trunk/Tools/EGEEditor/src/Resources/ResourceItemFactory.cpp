#include "ResourceItemFactory.h"
#include "ResourceItem.h"
#include "ResourceItemContainer.h"
#include "ResourceItemImage.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
struct BuiltInResourceItem
{
  const char* name;
  resourceItemCreateFunc pfCreateFunc;
};

static BuiltInResourceItem l_resourceItemsToRegister[] = {  { "container", ResourceItemContainer::Create },
                                                            { "image", ResourceItemImage::Create }
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceItemFactory::ResourceItemFactory(QObject* parent) : QObject(parent)
{
  // register build-in emitter types
  for (int i = 0; i < sizeof (l_resourceItemsToRegister) / sizeof (BuiltInResourceItem); ++i)
  {
    const BuiltInResourceItem& item = l_resourceItemsToRegister[i];

    registerItem(item.name, item.pfCreateFunc);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceItemFactory::~ResourceItemFactory()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Registeres custom resource item type. */
bool ResourceItemFactory::registerItem(const QString& typeName, resourceItemCreateFunc createFunc)
{
  // check if resource with such a name exists already
  if (isItemRegistered(typeName))
  {
    // error!
    return false;
  }

  // register
  m_registeredItems.insert(typeName, createFunc);

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance of resource item of the type given by name. */
ResourceItem* ResourceItemFactory::createItem(const QString& typeName, const QString& name, ResourceItem* parent) const
{
  ResourceItem* item = NULL;

  // get resource item create function for a given type name
  resourceItemCreateFunc createFunc = m_registeredItems.value(typeName, NULL);
  if (createFunc)
  {
    // create instance
    item = createFunc();
    if (item)
    {
      item->setParent(parent);
      item->setName(name);
    }
  }

  return item;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if given resource item type is registered. */
bool ResourceItemFactory::isItemRegistered(const QString& typeName) const
{
  return m_registeredItems.contains(typeName);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
