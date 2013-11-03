#ifndef RESOURCELIBRARY_RESOURCEITEMFACTORY_H
#define RESOURCELIBRARY_RESOURCEITEMFACTORY_H

/*! Factory class for resource items. 
 */

#include <QString>
#include <QList>
#include <QObject>
#include "ResouceLibraryPlugin_global.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceItem;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef ResourceItem* (*FPRESOURCEITEMCREATEFUNC) (const QString& name, ResourceItem* parent);
typedef QString       (*FPRESOURCEITEMTYPENAMEFUNC) ();
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class RESOUCELIBRARYPLUGIN_API ResourceItemFactory : public QObject
{
  Q_OBJECT

  public:

    explicit ResourceItemFactory(QObject* parent = NULL);
   ~ResourceItemFactory();

    /*! Registeres custom resource item type. */
    bool registerItem(FPRESOURCEITEMTYPENAMEFUNC typeNameFunc, FPRESOURCEITEMCREATEFUNC createFunc);
    /*! Creates instance of resource item of the type given by name. */
    ResourceItem* createItem(const QString& typeName, const QString& name, ResourceItem* parent = NULL) const;
    /*! Returns TRUE if given resource item type is registered. */
    bool isItemRegistered(const QString& typeName) const;

  private:

    /*! Data struct of registered items. */
    struct ItemData
    {
      FPRESOURCEITEMTYPENAMEFUNC typeNameFunc;  /*!< Item type name function. */
      FPRESOURCEITEMCREATEFUNC   createFunc;    /*!< Item create function. */
    };

    typedef QList<ItemData> ItemRegisterList;

  private:

    /*! List of registered items. */
    ItemRegisterList m_registeredItems;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // RESOURCELIBRARY_RESOURCEITEMFACTORY_H
