#ifndef RESOURCE_ITEM_FACTORY_H
#define RESOURCE_ITEM_FACTORY_H

#include <QString>
#include <QMap>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class ResourceItem;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

typedef ResourceItem* (*resourceItemCreateFunc) ();

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceItemFactory
{
  public:

    ResourceItemFactory();
   ~ResourceItemFactory();

    /* Registeres custom resource item type. */
    bool registerItem(const QString& typeName, resourceItemCreateFunc createFunc);
    /* Creates instance of resource item of the type given by name. */
    ResourceItem* createItem(const QString& typeName, const QString& name, ResourceItem* parent = NULL) const;
    /* Returns TRUE if given resource item type is registered. */
    bool isItemRegistered(const QString& typeName) const;

  private:

    typedef QMap<QString, resourceItemCreateFunc> ResourceItemRegisterMap;

  private:

    /*! Registered items sorted by type name. */
    ResourceItemRegisterMap m_registeredItems;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // RESOURCE_ITEM_FACTORY_H