#include "FilePathEditFactory.h"
#include "FilePathEditWidget.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
FilePathEditFactory::FilePathEditFactory(QObject* parent) : QtAbstractEditorFactory<FilePathPropertyManager>(parent)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
FilePathEditFactory::~FilePathEditFactory()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void FilePathEditFactory::connectPropertyManager(FilePathPropertyManager* manager)
{
  Q_UNUSED(manager);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void FilePathEditFactory::disconnectPropertyManager(FilePathPropertyManager* manager)
{
  Q_UNUSED(manager);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QWidget* FilePathEditFactory::createEditor(FilePathPropertyManager* manager, QtProperty* property, QWidget* parent)
{
  // create editor
  FilePathEditWidget* editor = new FilePathEditWidget(tr("Select texture image"), parent);

  // setup editor
  editor->setPath(manager->value(property));
  editor->setFilters(manager->filter(property));

  // add to pool
  m_editorToProperty[editor]    = property;
  m_propertyToEditor[property]  = editor;

  // connect
  connect(editor, SIGNAL(filePathChanged(const QString&)), this, SLOT(onFilePathChanged(const QString&)));
  connect(editor, SIGNAL(destroyed(QObject*)), this, SLOT(onEditorDestroyed(QObject*)));

  return editor;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void FilePathEditFactory::onEditorDestroyed(QObject* object)
{
  // NOTE: qobject_cast will not work here as object is already destroyed and only QObject shell still remains.
  FilePathEditWidget* editor = reinterpret_cast<FilePathEditWidget*>(object);

  // clean up pools
  m_propertyToEditor.remove(m_editorToProperty[editor]);
  m_editorToProperty.remove(editor);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void FilePathEditFactory::onFilePathChanged(const QString& path)
{
  FilePathEditWidget* editor = qobject_cast<FilePathEditWidget*>(sender());

  QtProperty* property = m_editorToProperty[editor];

  FilePathPropertyManager* manager = qobject_cast<FilePathPropertyManager*>(property->propertyManager());
  Q_ASSERT(NULL != manager);

  // update property data
  manager->setValue(property, path);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
