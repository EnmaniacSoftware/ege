#include "DirectoryPathEditFactory.h"
#include "DirectoryPathEditWidget.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DirectoryPathEditFactory::DirectoryPathEditFactory(QObject* parent) : QtAbstractEditorFactory<DirectoryPathPropertyManager>(parent)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DirectoryPathEditFactory::~DirectoryPathEditFactory()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void DirectoryPathEditFactory::connectPropertyManager(DirectoryPathPropertyManager* manager)
{
  Q_UNUSED(manager);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void DirectoryPathEditFactory::disconnectPropertyManager(DirectoryPathPropertyManager* manager)
{
  Q_UNUSED(manager);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QWidget* DirectoryPathEditFactory::createEditor(DirectoryPathPropertyManager* manager, QtProperty* property, QWidget* parent)
{
  // create editor
  DirectoryPathEditWidget* editor = new DirectoryPathEditWidget(tr("Select directory"), parent);

  // setup editor
  editor->setPath(manager->value(property));

  // add to pool
  m_editorToProperty[editor]    = property;
  m_propertyToEditor[property]  = editor;

  // connect
  connect(editor, SIGNAL(directoryPathChanged(const QString&)), this, SLOT(onDirectoryPathChanged(const QString&)));
  connect(editor, SIGNAL(destroyed(QObject*)), this, SLOT(onEditorDestroyed(QObject*)));

  return editor;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void DirectoryPathEditFactory::onEditorDestroyed(QObject* object)
{
  // NOTE: qobject_cast will not work here as object is already destroyed and only QObject shell still remains.
  DirectoryPathEditWidget* editor = reinterpret_cast<DirectoryPathEditWidget*>(object);

  // clean up pools
  m_propertyToEditor.remove(m_editorToProperty[editor]);
  m_editorToProperty.remove(editor);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void DirectoryPathEditFactory::onDirectoryPathChanged(const QString& path)
{
  DirectoryPathEditWidget* editor = qobject_cast<DirectoryPathEditWidget*>(sender());

  QtProperty* property = m_editorToProperty[editor];

  DirectoryPathPropertyManager* manager = qobject_cast<DirectoryPathPropertyManager*>(property->propertyManager());
  Q_ASSERT(NULL != manager);

  // update property data
  manager->setValue(property, path);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
