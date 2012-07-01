#ifndef ATLAS_GROUP_ENTRY_H
#define ATLAS_GROUP_ENTRY_H

#include <QString>
#include <QImage>
#include <QVector4D>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class AtlasGroupEntry
{
  public:

    AtlasGroupEntry(const QString& name, const QString& path, const QVector4D& spacing);
   ~AtlasGroupEntry();

    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /*! Returns image object. */
    inline QImage image() const { return m_image; }
    /*! Returns image path. */
    inline const QString& path() const { return m_path; }
    /*! Returns image name. */
    inline const QString& name() const { return m_name; }
    /*! Returns spacing. */
    inline const QVector4D& spacing() const { return m_spacing; }

  private:

    /*! Image name. */
    QString m_name;
    /*! Path to image. */
    QString m_path;
    /*! Spacing (left, top, right, bottom). */
    QVector4D m_spacing;
    /*! Image. */
    QImage m_image;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // ATLAS_GROUP_ENTRY_H