#include "Project.h"
#include "MainWindow.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Project::Project(QObject* parent, const QString& name, const QString& path) : QObject(parent),
                                                                              m_name(name),
                                                                              m_path(path),
                                                                              m_dirty(true),
                                                                              m_resourceLibraryItemDelegate(NULL)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Project::~Project()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Marks/unmarks project content as dirty. */
void Project::setDirty(bool set)
{
  m_dirty = set;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

/*! ISerializer override. Serializes into given stream. */
//bool Project::serialize(QXmlStreamWriter& stream) const
//{
//  // reset flag
//  m_dirty = false;
//
//  return true;
//}
////--------------------------------------------------------------------------------------------------------------------------------------------------------------
///*! ISerializer override. Unserializes from given data stream. */
//bool Project::unserialize(QXmlStreamReader& stream)
//{
//  return false;
//}
////--------------------------------------------------------------------------------------------------------------------------------------------------------------
