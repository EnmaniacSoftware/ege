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

    ResourceManager(const QString& baseName, float scale, QObject* parent = NULL);
   ~ResourceManager();

    /* Adds image into pool. 
       @param image Image to be added.
       @return  On success, non-negative unique image identifier assigned to given image. Otherwise, negative.
     */
    int addImage(const QImage& image);
    /* Serializes into EGE XML. */
    bool serialize(QXmlStreamWriter& stream);

  private:

    /*! Image pool. */
    QList<QImage> m_images;
    /*! Base name for materials and images. */
    QString m_baseName;
    /*! Scale factor. */
    float m_scale;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // RESOURCE_MANAGER_H
