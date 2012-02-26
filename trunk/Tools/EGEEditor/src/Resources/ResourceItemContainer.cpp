#include "ResourceItemContainer.h"
#include "ResourceLibraryDataModel.h"
#include <QImageReader>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceItemContainer::ResourceItemContainer() : ResourceItem()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceItemContainer::~ResourceItemContainer()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates instance of resource item. This method is a registration method for factory. */
ResourceItem* ResourceItemContainer::Create()
{
  return new ResourceItemContainer();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! ResourceItem override. Returns the item flags for the given item. */
Qt::ItemFlags ResourceItemContainer::flags() const
{
  return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! ResourceItem override. Returns type name. */
QString ResourceItemContainer::type() const
{
  return "container";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
