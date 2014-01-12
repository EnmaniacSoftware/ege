#ifndef WORKSPACE_FILEPATHEDITFACTORY_H
#define WORSSPACE_FILEPATHEDITFACTORY_H

#include <QtAbstractEditorFactoryBase>
#include "FilePathPropertyManager.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class QtProperty;
class FilePathEditWidget;
class FilePathEditFactoryPrivate;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class FilePathEditFactory : public QtAbstractEditorFactory<FilePathPropertyManager>
{
  Q_OBJECT

  public:

    explicit FilePathEditFactory(QObject* parent = 0);
   ~FilePathEditFactory();

  protected:

    void connectPropertyManager(FilePathPropertyManager* manager);
    void disconnectPropertyManager(FilePathPropertyManager* manager);
    QWidget* createEditor(FilePathPropertyManager* manager, QtProperty* property, QWidget* parent);

  private slots:

    /*! Slot called by manager when value (file path) of the property changes.
     *  @param  property  Property which value changed.
     *  @param  value     New value of the property (file path).
     */
    void onValueChanged(QtProperty* property, const QString& value);
    /*! Slot called by manager when filter of the property changes.
     *  @param  property  Property which value changed.
     *  @param  filter    New filter of the property.
     */
    void onFilterChanged(QtProperty* property, const QString& filter);
    /*! Slot called by editor when file path changes. */
    void onFilePathChanged(const QString& path);
    /*! Slot called when editor widget is being destroyed. */
    void onEditorDestroyed(QObject* object);

  private:

    /*! Property being edited. Can be NULL. */
    QtProperty* m_editedProperty;
    /*! Editor for property being edited. Can be NULL. */
    FilePathEditWidget* m_editor;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // WORKSPACE_FILEPATHEDITFACTORY_H
