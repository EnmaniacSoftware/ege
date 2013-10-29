#include "ProjectJuiceJumpers.h"
//#include <IResourceLibrary.h>
#include <ObjectPool.h>
//#include <ResourceLibraryItemDelegate.h>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ProjectJuiceJumpers::ProjectJuiceJumpers(QObject* parent, const QString& name, const QString& path) : Project(parent, TypeName(), name, path)
{
  //m_resourceLibraryItemDelegate = new ResourceLibraryItemDelegate(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ProjectJuiceJumpers::~ProjectJuiceJumpers()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Project* ProjectJuiceJumpers::Create(QObject* parent, const QString& name, const QString& path)
{
  return new ProjectJuiceJumpers(parent, name, path);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QString ProjectJuiceJumpers::TypeName()
{
  return "Juice Jumpers";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ProjectJuiceJumpers::serialize(QXmlStreamWriter& stream) const
{
  stream.writeStartElement("project");
  
  // project data
  stream.writeAttribute("name", name());
  stream.writeAttribute("path", path());
  stream.writeAttribute("type", TypeName());

  // serialize resources
//  IResourceLibrary* resourceLibrary = ObjectPool::Instance()->getObject<IResourceLibrary>();
  bool result = true;//resourceLibrary->serialize(stream);

  stream.writeEndElement();

  bool success = !stream.hasError() && result;

  // update flag
  //setDirty(!success);

  return success;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ProjectJuiceJumpers::unserialize(QXmlStreamReader& stream)
{
  // unserialize resources
//  IResourceLibrary* resourceLibrary = ObjectPool::Instance()->getObject<IResourceLibrary>();
  bool result = true;//resourceLibrary->unserialize(stream);

  return ! stream.hasError() && result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
