#ifndef CORE_PROJECTS_PROJECT_H
#define CORE_PROJECTS_PROJECT_H

/*! Project base class. 
 */

#include <QObject>
#include <QString>
#include "Serializer.h"
#include "CorePlugin_global.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//class MainWindow;
//class ResourceLibraryItemDelegate;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class COREPLUGIN_API Project : public QObject, public ISerializer
{
  Q_OBJECT

  public:

    virtual ~Project();

  signals:

    /*! Signal emitted when project dirty flag changes. */
    void dirtyFlagChanged();

  public:

    /*! Returns type name. */
    const QString& typeName() const;
    /*! Returns name. */
    const QString& name() const;
    /*! Returns path. */
    const QString& path() const;

    /*! Marks/unmarks project content as dirty. */
    void setDirty(bool set);
    /*! Returns dirty flag. */
    bool isDirty() const;
    
    /*! Returns resource library item delegate. */
  //  inline ResourceLibraryItemDelegate* resourceLibraryItemDelegate() const { return m_resourceLibraryItemDelegate; }
    
  public slots:

    /*! Slot called when project data has changed. */
    void onProjectDataChanged();

  protected:

    /*! Returns pointer to main window object. */
    //inline MainWindow* mainWindow() const { return reinterpret_cast<MainWindow*>(parent()); }

  protected:

    /*! Constructor. */
    Project(QObject* parent, const QString& typeName, const QString& name, const QString& path);
    /*! Resource library item delegate. */
   // ResourceLibraryItemDelegate* m_resourceLibraryItemDelegate;

  private:

    /*! Type name. */
    QString m_typeName;
    /*! Name. */
    QString m_name;
    /*! Path to project file. */
    QString m_path;
    /*! Dirty flag. */
    mutable bool m_dirty;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // CORE_PROJECTS_PROJECT_H
