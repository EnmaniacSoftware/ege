#ifndef PROJECT_H
#define PROJECT_H

#include <QObject>
#include <QString>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class MainWindow;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Project : public QObject
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
    /* Marks/unmarks project content as dirty. */
    void setDirty(bool set);
    /*! Returns dirty flag. */
    inline bool isDirty() const { return m_dirty; }
    /* Saves all data. */
    virtual bool save();

  protected:

    /*! Returns pointer to main window object. */
    inline MainWindow* mainWindow() const { return reinterpret_cast<MainWindow*>(parent()); }

  private:

    /*! Name. */
    QString m_name;
    /*! Path to project file. */
    QString m_path;
    /*! Dirty flag. */
    bool m_dirty;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef Project* (*projectCreateFunc)(QObject* parent, const QString& name, const QString& path);
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // PROJECT_H