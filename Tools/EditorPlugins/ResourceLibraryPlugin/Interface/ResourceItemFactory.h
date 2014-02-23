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
class QMenu;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef ResourceItem* (*FPRESOURCEITEMCREATEFUNC) (const QString& name, const QString& configurationName, const QUuid& id, ResourceItem* parent);
typedef QString       (*FPRESOURCEITEMTYPENAMEFUNC) ();
typedef void          (*FPRESOURCEITEMRESLIBWNDCONTEXTMENUHOOKFUNC) (QMenu& menu, const QString& selectedType);
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class RESOUCELIBRARYPLUGIN_API ResourceItemFactory : public QObject
{
  Q_OBJECT

  public:

    explicit ResourceItemFactory(QObject* parent = NULL);
   ~ResourceItemFactory();

  public:

    /*! Data struct of registered items. */
    struct ItemData
    {
      FPRESOURCEITEMTYPENAMEFUNC typeNameFunc;                                  /*!< Item type name function. */
      FPRESOURCEITEMCREATEFUNC   createFunc;                                    /*!< Item create function. */
      FPRESOURCEITEMRESLIBWNDCONTEXTMENUHOOKFUNC resourceLibraryWindowHookFunc; /*!< Item hook function for Resource Library Window context menu. */
    };

  public:

    /*! Registeres custom resource item type.
     *  @param  typeNameFunc                  Global function returning resource item type name.
     *  @param  createFunc                    Global function returning new instance of a given resource item.
     *  @param  resourceLibraryWindowHookFunc Global function used to hook into Resource Library Window context menu.
     */
    bool registerItem(FPRESOURCEITEMTYPENAMEFUNC typeNameFunc, FPRESOURCEITEMCREATEFUNC createFunc,
                      FPRESOURCEITEMRESLIBWNDCONTEXTMENUHOOKFUNC resourceLibraryWindowHookFunc);
    /*! Returns TRUE if given resource item type is registered. */
    bool isItemRegistered(const QString& typeName) const;

    /*! Creates instance of resource item.
     *  @param  typeName          Type of the item to be created.
     *  @param  name              Name of the item.
     *  @param  configurationName Name of the configuration for which item is being created.
     *  @param  id                Object ID.
     *  @param  parent            Parent item.
     *  @return Created item. NULL for failure.
     */
    ResourceItem* createItem(const QString& typeName, const QString& name, const QString& configurationName, const QUuid& id,
                             ResourceItem* parent = NULL) const;

    /*! Returns list of registred items. */
    const QList<ItemData>& items() const;

  private:

    typedef QList<ItemData> ItemRegisterList;

  private:

    /*! List of registered items. */
    ItemRegisterList m_registeredItems;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // RESOURCELIBRARY_RESOURCEITEMFACTORY_H
