#ifndef PROJECT_H
#define PROJECT_H

#include <QObject>
#include <QString>
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Project : public QObject
{
  Q_OBJECT

  public:

    Project(QObject* parent, const QString& name, const QString& path);
    ~Project();

    /*! Returns name. */
    inline const QString& name() const { return m_name; }
    /*! Returns path. */
    inline const QString& path() const { return m_path; }

  private:

    /*! Name. */
    QString m_name;
    /*! Path to project file. */
    QString m_path;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef Project* (*projectCreateFunc)(QObject* parent, const QString& name, const QString& path);
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // PROJECT_H