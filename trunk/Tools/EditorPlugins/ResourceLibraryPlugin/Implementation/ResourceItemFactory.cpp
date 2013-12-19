#include "ResourceItemFactory.h"
#include "ResourceItem.h"
#include "ResourceItemContainer.h"
#include "ResourceItemImage.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
struct BuiltInResourceItem
{
  FPRESOURCEITEMTYPENAMEFUNC                 pfTypeNameFunc;
  FPRESOURCEITEMCREATEFUNC                   pfCreateFunc;
  FPRESOURCEITEMRESLIBWNDCONTEXTMENUHOOKFUNC pfResourceLibraryWindowHookFunc;
};

static BuiltInResourceItem l_resourceItemsToRegister[] = {  { ResourceItemContainer::TypeName,
                                                              ResourceItemContainer::Create,
                                                              ResourceItemContainer::ResourceLibraryWindowHook
                                                            },
                                                            { ResourceItemImage::TypeName,
                                                              ResourceItemImage::Create,
                                                              ResourceItemImage::ResourceLibraryWindowHook
                                                            }
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceItemFactory::ResourceItemFactory(QObject* parent) : QObject(parent)
{
  // register build-in emitter types
  for (int i = 0; i < sizeof (l_resourceItemsToRegister) / sizeof (BuiltInResourceItem); ++i)
  {
    const BuiltInResourceItem& item = l_resourceItemsToRegister[i];

    registerItem(item.pfTypeNameFunc, item.pfCreateFunc, item.pfResourceLibraryWindowHookFunc);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceItemFactory::~ResourceItemFactory()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ResourceItemFactory::registerItem(FPRESOURCEITEMTYPENAMEFUNC typeNameFunc, FPRESOURCEITEMCREATEFUNC createFunc,
                                       FPRESOURCEITEMRESLIBWNDCONTEXTMENUHOOKFUNC resourceLibraryWindowHookFunc)
{
  Q_ASSERT((NULL != typeNameFunc) && (NULL != createFunc) && (NULL != resourceLibraryWindowHookFunc));

  // check if resource with such a name exists already
  if (isItemRegistered(typeNameFunc()))
  {
    // error!
    return false;
  }

  // register
  ItemData data;
  data.createFunc                    = createFunc;
  data.typeNameFunc                  = typeNameFunc;
  data.resourceLibraryWindowHookFunc = resourceLibraryWindowHookFunc;

  m_registeredItems.append(data);

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceItem* ResourceItemFactory::createItem(const QString& typeName, const QString& name, ResourceItem* parent) const
{
  ResourceItem* item = NULL;

  // get resource item create function for a given type name
  foreach (const ItemData& itemData, m_registeredItems)
  {
    if (itemData.typeNameFunc() == typeName)
    {
      // create instance
      item = itemData.createFunc(name, parent);
      break;
    }
  }

  return item;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ResourceItemFactory::isItemRegistered(const QString& typeName) const
{
  foreach (const ItemData& itemData, m_registeredItems)
  {
    if (itemData.typeNameFunc() == typeName)
    {
      // found
      return true;
    }
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const QList<ResourceItemFactory::ItemData>& ResourceItemFactory::items() const
{
  return m_registeredItems;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
