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
/*! Returns type name. */
QString ProjectJuiceJumpers::TypeName()
{
  return "Juice Jumpers";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

/*! Project override. Serializes into given stream. */
bool ProjectJuiceJumpers::serialize(QXmlStreamWriter& stream) const
{
  stream.writeStartElement("project");
  
  stream.writeAttribute("name", name());
  stream.writeAttribute("path", path());
  stream.writeAttribute("type", TypeName());

  stream.writeEndElement();

  // serialize children
  //foreach (const ResourceItem* item, m_children)
  //{
  //  if (!item->serialize(stream))
  //  {
  //    // error!
  //    return false;
  //  }
  //}

  return !stream.hasError();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Project override. Unserializes from given data stream. */
bool ProjectJuiceJumpers::unserialize(QXmlStreamReader& stream)
{
  //// call base class
  //if (!Project::unserialize(stream))
  //{
  //  // error!
  //  return false;
  //}

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
