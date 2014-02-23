#include "JuiceJumpersProject.h"
#include "JuiceJumpersResourceLibraryItemDelegate.h"
#include <ObjectPool.h>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
JuiceJumpersProject::JuiceJumpersProject(QObject* parent, const QString& name, const QString& path)
: Project(parent, TypeName(), name, path),
  m_resourceItemDelegate(new JuiceJumpersResourceLibraryItemDelegate(this))
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
JuiceJumpersProject::~JuiceJumpersProject()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Project* JuiceJumpersProject::Create(QObject* parent, const QString& name, const QString& path)
{
  return new JuiceJumpersProject(parent, name, path);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QString JuiceJumpersProject::TypeName()
{
  return "Juice Jumpers";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool JuiceJumpersProject::serialize(QXmlStreamWriter& stream) const
{
  // serialize resources
//  IResourceLibrary* resourceLibrary = ObjectPool::Instance()->getObject<IResourceLibrary>();
  bool result = true;//resourceLibrary->serialize(stream);

  bool success = !stream.hasError() && result;

  // update flag
  //setDirty(!success);

  return success;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool JuiceJumpersProject::unserialize(QXmlStreamReader& stream)
{
  // unserialize resources
//  IResourceLibrary* resourceLibrary = ObjectPool::Instance()->getObject<IResourceLibrary>();
  bool result = true;//resourceLibrary->unserialize(stream);

  return ! stream.hasError() && result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QStyledItemDelegate* JuiceJumpersProject::resourceLibraryItemDelegate() const
{
  return m_resourceItemDelegate;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
