#ifndef WORKSPACE_FILEPATHEDITFACTORY_H
#define WORSSPACE_FILEPATHEDITFACTORY_H

/*! Factory class responsible for creation of editable widgets for manipulating FilePath types of QtProperties presented within QtPropertyViews.
 */

#include <QtAbstractEditorFactoryBase>
#include <QMap>
#include "FilePathPropertyManager.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class QtProperty;
class FilePathEditWidget;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class FilePathEditFactory : public QtAbstractEditorFactory<FilePathPropertyManager>
{
  Q_OBJECT

  public:

    explicit FilePathEditFactory(QObject* parent = 0);
   ~FilePathEditFactory();

  protected:

    /*! @see QtAbstractEditorFactory::connectPropertyManager. */
    void connectPropertyManager(FilePathPropertyManager* manager);
    /*! @see QtAbstractEditorFactory::disconnectPropertyManager. */
    void disconnectPropertyManager(FilePathPropertyManager* manager);
    /*! @see QtAbstractEditorFactory::createEditor. */
    QWidget* createEditor(FilePathPropertyManager* manager, QtProperty* property, QWidget* parent);

  private slots:

    /*! Slot called by editor when file path changes. */
    void onFilePathChanged(const QString& path);
    /*! Slot called when editor widget is being destroyed. */
    void onEditorDestroyed(QObject* object);

  private:

    /*! Look up for created editor based on property being edited. */
    QMap<QtProperty*, FilePathEditWidget*> m_propertyToEditor;
    /*! Look up for property being edited based on editor which edits it. */
    QMap<FilePathEditWidget*, QtProperty*> m_editorToProperty;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // WORKSPACE_FILEPATHEDITFACTORY_H
