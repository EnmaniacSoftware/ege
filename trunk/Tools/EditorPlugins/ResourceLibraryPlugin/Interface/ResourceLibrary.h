#ifndef RESOURCELIBRARY_RESOURCELIBRARY_H
#define RESOURCELIBRARY_RESOURCELIBRARY_H

/*! Resource library. This object contains a collection of resource items available to current project.
 *  Contained resources are shared between all configurations.
 */

#include <QObject>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceLibraryDataModel;
class QXmlStreamWriter;
class QXmlStreamReader;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceLibrary : public QObject
{
  Q_OBJECT

  public:

    explicit ResourceLibrary(QObject* parent = NULL);
   ~ResourceLibrary();

  signals:

    /*! Signal emitted when data loading has completed.
     *  @param  count Number of loaded items.
     */
    void loaded(int count);

  public:

    /*! Returns model. */
    ResourceLibraryDataModel* model() const;

  private slots:

    /*! Slot called when library saving is required.
     *  @param  stream  XML stream where data is to be saved.
     */
    void onSaveData(QXmlStreamWriter& stream);
    /*! Slot called when library loading is required.
     *  @param  stream  XML stream where data is to be saved.
     */
    void onLoadData(QXmlStreamReader& stream);

  private:

    /*! Data model. */
    ResourceLibraryDataModel* m_model;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // RESOURCELIBRARY_RESOURCELIBRARY_H
