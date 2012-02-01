#include "ResourceLibraryItem.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceLibraryItem::ResourceLibraryItem(const QString& name, const QString& path, Type type) : m_parent(NULL),
                                                                                                m_name(name),
                                                                                                m_path(path),
                                                                                                m_type(type)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResourceLibraryItem::~ResourceLibraryItem()
{
  qDeleteAll(m_children);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns number of columns. */
int ResourceLibraryItem::columnCount() const
{
  return 1;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns children count. */
int ResourceLibraryItem::childCount() const
{
  return m_children.count();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns row index at which current item is placed withing parent. */
int ResourceLibraryItem::row() const
{
  if (NULL != m_parent)
  {
    return m_parent->m_children.indexOf(const_cast<ResourceLibraryItem*>(this));
  }

  return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns data for a given column. */
QVariant ResourceLibraryItem::data(int columnIndex) const
{
  return m_name;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Adds child. */
void ResourceLibraryItem::add(ResourceLibraryItem* child)
{
  // set parent
  child->m_parent = this;

  // append
  m_children.append(child);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns child at given index. */
ResourceLibraryItem* ResourceLibraryItem::child(int index) const
{
  return ((0 <= index) && (index < childCount())) ? m_children[index] : NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
