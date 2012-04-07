#include "Project.h"
#include "MainWindow.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Project::Project(QObject* parent, const QString& typeName, const QString& name, const QString& path) :  QObject(parent),
                                                                                                        m_typeName(typeName),
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
  if (m_dirty != set)
  {
    m_dirty = set;

    emit dirtyFlagChanged();
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when project data has changed. */
void Project::onProjectDataChanged()
{
  setDirty(true);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------