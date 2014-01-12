#include "FilePathEditFactory.h"
#include "FilePathEditWidget.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
FilePathEditFactory::FilePathEditFactory(QObject* parent) : QtAbstractEditorFactory<FilePathPropertyManager>(parent),
                                                            m_editedProperty(NULL),
                                                            m_editor(NULL)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
FilePathEditFactory::~FilePathEditFactory()
{

}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void FilePathEditFactory::connectPropertyManager(FilePathPropertyManager* manager)
{
  connect(manager, SIGNAL(valueChanged(QtProperty*, const QString&)), this, SLOT(onValueChanged(QtProperty*, const QString&)));
  connect(manager, SIGNAL(filterChanged(QtProperty*, const QString&)), this, SLOT(onFilterChanged(QtProperty*, const QString&)));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void FilePathEditFactory::disconnectPropertyManager(FilePathPropertyManager* manager)
{
  disconnect(manager, SIGNAL(valueChanged(QtProperty*, const QString&)), this, SLOT(onValueChanged(QtProperty*, const QString&)));
  disconnect(manager, SIGNAL(filterChanged(QtProperty*, const QString&)), this, SLOT(onFilterChanged(QtProperty*, const QString&)));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QWidget* FilePathEditFactory::createEditor(FilePathPropertyManager* manager, QtProperty* property, QWidget* parent)
{
  // create editor
  FilePathEditWidget* m_editor = new FilePathEditWidget(tr("Select texture image"), parent);

  // setup editor
  m_editor->setPath(manager->value(property));
  m_editor->setFilters(manager->filter(property));

  // store property being edited
  m_editedProperty = property;

  // connect
  connect(m_editor, SIGNAL(filePathChanged(const QString&)), this, SLOT(onFilePathChanged(const QString&)));
  connect(m_editor, SIGNAL(destroyed(QObject*)), this, SLOT(onEditorDestroyed(QObject*)));

  return m_editor;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void FilePathEditFactory::onValueChanged(QtProperty* property, const QString& value)
{

}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void FilePathEditFactory::onFilterChanged(QtProperty* property, const QString& filter)
{

}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void FilePathEditFactory::onEditorDestroyed(QObject* object)
{
  Q_ASSERT(object == m_editor);

  // clean up
  m_editedProperty = NULL;
  m_editor = NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void FilePathEditFactory::onFilePathChanged(const QString& path)
{
  FilePathEditWidget* editor = qobject_cast<FilePathEditWidget*>(sender());

  Q_ASSERT(editor == m_editor);

  FilePathPropertyManager* manager = qobject_cast<FilePathPropertyManager*>(m_editedProperty->propertyManager());
  Q_ASSERT(NULL != manager);

  // update property data
  manager->setValue(m_editedProperty, path);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
