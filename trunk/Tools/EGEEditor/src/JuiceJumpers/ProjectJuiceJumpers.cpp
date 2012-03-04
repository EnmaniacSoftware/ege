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
/*! Project override. Serializes into given stream. */
bool ProjectJuiceJumpers::serialize(QXmlStreamWriter& stream) const
{
  return Project::serialize(stream);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Project override. Unserializes from given data stream. */
bool ProjectJuiceJumpers::unserialize(const QXmlStreamReader& stream)
{
  // call base class
  if (!Project::unserialize(stream))
  {
    // error!
    return false;
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
