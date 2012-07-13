#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <QImage>
#include <QList>
#include <QXmlStreamWriter>
#include <QString>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! @brief Resource manager object. */
class ResourceManager : public QObject
{
  Q_OBJECT

  public:

    ResourceManager(const QString& baseName, const QString& outputLocation, float scale, QObject* parent = NULL);
   ~ResourceManager();

    /* Adds image into pool. 
       @param image Image to be added.
       @return  On success, non-negative unique image identifier assigned to given image. Otherwise, negative.
     */
    int addImage(const QImage& image);
    /* Serializes into EGE XML. */
    bool serialize(QXmlStreamWriter& stream);
    /* Generates asset's name from image ID. */
    QString generateNameFromImageId(quint16 id) const;
    /* Saves assets. */
    bool saveAssets();
    /* Returns image references by given id. */
    QImage image(int id) const;

  private:

    /*! Image pool. */
    QList<QImage> m_images;
    /*! Base name for materials and images. */
    QString m_baseName;
    /*! Scale factor. */
    float m_scale;
    /*! Output location path. */
    QString m_outputLocation;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // RESOURCE_MANAGER_H
