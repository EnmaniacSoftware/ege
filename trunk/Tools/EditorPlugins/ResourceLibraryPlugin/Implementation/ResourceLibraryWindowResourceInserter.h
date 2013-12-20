#ifndef RESOURCELIBRARY_RESOURCELIBRARYWINDOWRESOURCEADDER_H
#define RESOURCELIBRARY_RESOURCELIBRARYWINDOWRESOURCEADDER_H

/*! Helper class used as built-in resource items inserter for Resource Library Window context menu.
 */

#include <QString>
#include <QObject>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceLibrary;
class ResourceLibraryWindow;
class ResourceItemFactory;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceLibraryWindowResourceInserter : public QObject
{
  Q_OBJECT

  public:

    ResourceLibraryWindowResourceInserter(ResourceLibrary* library, ResourceLibraryWindow* libraryWindow, ResourceItemFactory* factory);
   ~ResourceLibraryWindowResourceInserter();

  public slots:

    /*! Slot called when new group resource is to be added. */
    void onAddGroup();
    /*! Slot called when new 2D texture resource is to be added. */
    void onAddTexture2D();

  private:

    /*! Generates valid group name.
     *  @note There can be no duplicate names within same configuration.
     */
    QString generateGroupName() const;

  private:

    /*! Pointer to resorce library. */
    ResourceLibrary* m_library;
    /*! Pointer to resource library window. */
    ResourceLibraryWindow* m_window;
    /*! Pointer to resource item factory. */
    ResourceItemFactory* m_factory;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // RESOURCELIBRARY_RESOURCELIBRARYWINDOWRESOURCEADDER_H
