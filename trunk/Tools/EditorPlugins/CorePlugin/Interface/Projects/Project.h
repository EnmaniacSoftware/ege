#ifndef CORE_PROJECTS_PROJECT_H
#define CORE_PROJECTS_PROJECT_H

/*! Project base class. 
 */

#include <QObject>
#include <QString>
#include "Serializer.h"
#include "CorePlugin_global.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class QStyledItemDelegate;
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
    virtual QStyledItemDelegate* resourceLibraryItemDelegate() const = 0;
    
  public slots:

    /*! Slot called when project data has changed. */
    void onProjectDataChanged();

  protected:

    /*! Constructor. */
    Project(QObject* parent, const QString& typeName, const QString& name, const QString& path);

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
