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
       @param image       Image to be added.
       @param characterId SWF character ID for an image.
       @return  On success, non-negative unique image identifier assigned to given image. Otherwise, negative.
     */
    int addImage(const QImage& image, quint16 characterId);
    /* Serializes into EGE XML. */
    bool serialize(QXmlStreamWriter& stream);
    /* Generates asset's name from character ID. */
    QString generateNameFromCharacterId(quint16 characterId) const;
    /* Saves assets. */
    bool saveAssets();
    /* Returns image references by given character ID. */
    QImage image(quint16 characterId) const;

  private:

    /*! Image data struct. */
    struct ImageData
    {
      QImage image;             /*!< Unique image. */
      quint16 characterId;      /*!< Character id assigned to image. */
    };

  private:

    /*! Image pool. */
    QList<ImageData> m_images;
    /*! Base name for materials and images. */
    QString m_baseName;
    /*! Scale factor. */
    float m_scale;
    /*! Output location path. */
    QString m_outputLocation;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // RESOURCE_MANAGER_H
