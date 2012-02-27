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
/*! Project override. Serializes into given buffer. */
QString ProjectJuiceJumpers::serialize() const
{
  // call base class
  QString data = Project::serialize();

  return data;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Project override. Unserializes from given data buffer. */
bool ProjectJuiceJumpers::unserialize(const QString& data)
{
  // call base class
  if (!Project::unserialize(data))
  {
    // error!
    return false;
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
