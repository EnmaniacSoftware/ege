#ifndef WORKSPACE_DIRECTORYPATHEDITFACTORY_H
#define WORKSPACE_DIRECTORYPATHEDITFACTORY_H

/*! Factory class responsible for creation of editable widgets for manipulating DirectoryPath types of QtProperties presented within QtPropertyViews.
 */

#include <QtAbstractEditorFactoryBase>
#include <QMap>
#include "DirectoryPathPropertyManager.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class QtProperty;
class DirectoryPathEditWidget;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class DirectoryPathEditFactory : public QtAbstractEditorFactory<DirectoryPathPropertyManager>
{
  Q_OBJECT

  public:

    explicit DirectoryPathEditFactory(QObject* parent = 0);
   ~DirectoryPathEditFactory();

  protected:

    /*! @see QtAbstractEditorFactory::connectPropertyManager. */
    void connectPropertyManager(DirectoryPathPropertyManager* manager);
    /*! @see QtAbstractEditorFactory::disconnectPropertyManager. */
    void disconnectPropertyManager(DirectoryPathPropertyManager* manager);
    /*! @see QtAbstractEditorFactory::createEditor. */
    QWidget* createEditor(DirectoryPathPropertyManager* manager, QtProperty* property, QWidget* parent);

  private slots:

    /*! Slot called by editor when directory path changes. */
    void onDirectoryPathChanged(const QString& path);
    /*! Slot called when editor widget is being destroyed. */
    void onEditorDestroyed(QObject* object);

  private:

    /*! Look up for created editor based on property being edited. */
    QMap<QtProperty*, DirectoryPathEditWidget*> m_propertyToEditor;
    /*! Look up for property being edited based on editor which edits it. */
    QMap<DirectoryPathEditWidget*, QtProperty*> m_editorToProperty;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // WORKSPACE_DIRECTORYPATHEDITFACTORY_H
