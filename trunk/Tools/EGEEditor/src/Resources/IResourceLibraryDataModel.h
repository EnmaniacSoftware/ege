#ifndef RESOURCE_LIBRARY_DATA_MODEL_INTERFACE_H
#define RESOURCE_LIBRARY_DATA_MODEL_INTERFACE_H

#include <QList>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceItem;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class IResourceLibraryDataModel
{
  public:

    IResourceLibraryDataModel(QObject* object) : m_object(object) {}

    /*! Signal emitted when given item has been added to model. */
    virtual void onItemAdded(ResourceItem* item) = 0;
    /*! Signal emitted when given item has been removed from model. */
    virtual void onItemRemoved(ResourceItem* item) = 0;
    /*! Returns list of all items in the model. */
    virtual QList<ResourceItem*> allItems() const = 0;
    /*! Returns interfaced object. */
    inline QObject* object() const { return m_object; }

  private:

    /*! Interfaced object. */
    QObject* m_object;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // RESOURCE_LIBRARY_DATA_MODEL_INTERFACE_H