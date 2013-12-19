#ifndef RESOURCELIBRARY_RESOURCELIBRARYWINDOWGROUPADDER_H
#define RESOURCELIBRARY_RESOURCELIBRARYWINDOWGROUPADDER_H

#include <QString>
#include <QObject>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceLibraryWindowGroupAdder : public QObject
{
  Q_OBJECT

  public:

    ResourceLibraryWindowGroupAdder();
   ~ResourceLibraryWindowGroupAdder();

  public slots:

    /*! Slot called when new group resource is to be added. */
    void onAdd();

  private:

    /*! Generates valid group name.
     *  @note There can be no duplicate names within same configuration.
     */
    QString generateGroupName() const;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // RESOURCELIBRARY_RESOURCELIBRARYWINDOWGROUPADDER_H
