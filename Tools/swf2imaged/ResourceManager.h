#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <QImage>
#include <QList>
#include <QXmlStreamWriter>
#include <QString>
#include <QMap>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! @brief Resource manager object. */
class ResourceManager : public QObject
{
  Q_OBJECT

  public:

    ResourceManager(QObject* parent = NULL);
   ~ResourceManager();

    /* Adds image into pool. 
       @param image Image to be added.
       @return  On success, non-negative unique image identifier assigned to given image. Otherwise, negative.
     */
    int addImage(const QImage& image);
    /* Serializes into EGE XML. */
    bool serialize(QXmlStreamWriter& stream);
    /* Generates asset's name from image ID. */
    QString generateNameFromImageId(int id) const;
    /* Saves assets. */
    bool saveAssets();
    /* Returns image references by given id. */
    QImage image(int id) const;

    /* Adds atlas texture definition. */
    bool addAtlasTextureDefinition(const QMap<QString, QString>& properties);

    /* Begins new resource session. */
    bool beginSession(const QString& baseName, float scale, const QString& outputLocation, const QString& atlasTexture);

    /* Returns image index within current session from global image id. */
    int imageIndex(int id) const;

  private:
    
    /* Generates atlas texture XML. */
    bool generateAtlasTextureXML();

  private:

    /*! Atlas texture data struct. */
    struct AtlasTextureData
    {
      QMap<QString, QString> properties;
    };

    typedef QList<AtlasTextureData> AtlasTextureDataList;

    /*! Session data struct. */
    struct SessionData
    {
      QString baseName;
      QString outputLocation;
      QString atlasTexture;
      float scale;

      QList<int> imageList;
    };

    typedef QList<SessionData> SessionDataList;

  private:

    /*! Image pool. */
    QList<QImage> m_images;
    /*! List of declared sessions. */
    SessionDataList m_sessionList;
    /*! List of declared atlas textures. */
    AtlasTextureDataList m_atlasTexturesList;
    /*! Session in use. */
    SessionData* m_currentSession;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // RESOURCE_MANAGER_H
