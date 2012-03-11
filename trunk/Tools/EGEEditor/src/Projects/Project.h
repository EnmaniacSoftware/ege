#ifndef PROJECT_H
#define PROJECT_H

/*! Project base class. 
 */

#include <QObject>
#include <QString>
#include "Serializer.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class MainWindow;
class ResourceLibraryItemDelegate;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Project : public QObject, public ISerializer
{
  Q_OBJECT

  public:

    Project(QObject* parent, const QString& name, const QString& path);
    virtual ~Project();

  signals:

    /* Signal emitted when project dirty flag changes. */
    void dirtyFlagChanged();

  public:

    /*! Returns name. */
    inline const QString& name() const { return m_name; }
    
    /*! Returns path. */
    inline const QString& path() const { return m_path; }
    /*! Returns full path to project file. */
    inline QString fullPath() const { return m_path + "/" + m_name + ".ege"; }

    /* Marks/unmarks project content as dirty. */
    void setDirty(bool set);
    /*! Returns dirty flag. */
    inline bool isDirty() const { return m_dirty; }
    
    /*! Returns resource library item delegate. */
    inline ResourceLibraryItemDelegate* resourceLibraryItemDelegate() const { return m_resourceLibraryItemDelegate; }

  protected:

    /*! Returns pointer to main window object. */
    inline MainWindow* mainWindow() const { return reinterpret_cast<MainWindow*>(parent()); }

  protected:

    /*! Resource library item delegate. */
    ResourceLibraryItemDelegate* m_resourceLibraryItemDelegate;

  private:

    /*! Name. */
    QString m_name;
    /*! Path to project file. */
    QString m_path;
    /*! Dirty flag. */
    mutable bool m_dirty;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // PROJECT_H