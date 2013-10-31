#ifndef RESOURCELIBRARY_RESOURCEITEMFACTORY_H
#define RESOURCELIBRARY_RESOURCEITEMFACTORY_H

/*! Factory class for resource items. 
 */

#include <QString>
#include <QMap>
#include <QObject>
#include "ResouceLibraryPlugin_global.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceItem;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef ResourceItem* (*FPRESOURCEITEMCREATEFUNC) ();
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class RESOUCELIBRARYPLUGIN_API ResourceItemFactory : public QObject
{
  Q_OBJECT

  public:

    explicit ResourceItemFactory(QObject* parent = NULL);
   ~ResourceItemFactory();

    /*! Registeres custom resource item type. */
    bool registerItem(const QString& typeName, FPRESOURCEITEMCREATEFUNC createFunc);
    /*! Creates instance of resource item of the type given by name. */
    ResourceItem* createItem(const QString& typeName, const QString& name, ResourceItem* parent = NULL) const;
    /*! Returns TRUE if given resource item type is registered. */
    bool isItemRegistered(const QString& typeName) const;

  private:

    typedef QMap<QString, FPRESOURCEITEMCREATEFUNC> ResourceItemRegisterMap;

  private:

    /*! Registered items sorted by type name. */
    ResourceItemRegisterMap m_registeredItems;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // RESOURCELIBRARY_RESOURCEITEMFACTORY_H
