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
    inline const QString& typeName() const { return m_typeName; }
    /*! Returns name. */
    inline const QString& name() const { return m_name; }
    /*! Returns path. */
    inline const QString& path() const { return m_path; }
    /*! Returns full path to project file. */
    inline QString fullPath() const { return m_path + "/" + m_name + ".ege"; }

    /*! Marks/unmarks project content as dirty. */
    void setDirty(bool set);
    /*! Returns dirty flag. */
    inline bool isDirty() const { return m_dirty; }
    
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
