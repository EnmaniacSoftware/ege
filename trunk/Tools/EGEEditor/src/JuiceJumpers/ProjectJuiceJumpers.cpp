#include "ProjectJuiceJumpers.h"
#include "Resources/ResourceLibraryItemDelegate.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ProjectJuiceJumpers::ProjectJuiceJumpers(QObject* parent, const QString& name, const QString& path) : Project(parent, name, path)
{
  m_resourceLibraryItemDelegate = new ResourceLibraryItemDelegate(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ProjectJuiceJumpers::~ProjectJuiceJumpers()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Create method. */
Project* ProjectJuiceJumpers::Create(QObject* parent, const QString& name, const QString& path)
{
  return new ProjectJuiceJumpers(parent, name, path);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Project override. Saves all data. */
bool ProjectJuiceJumpers::save()
{
  // call base class
  if (!Project::save())
  {
    // error!
    return false;
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
