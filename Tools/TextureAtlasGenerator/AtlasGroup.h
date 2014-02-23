#ifndef ATLAS_GROUP_H
#define ATLAS_GROUP_H

#include <QString>
#include <QImage>
#include <QList>
#include <QSize>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class AtlasGroupEntry;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class AtlasGroup
{
  public:

    AtlasGroup(const QString& name, const QString& root, const QString& textureImageName, const QString& textureFormatName, const QString& textureFiltersName, 
               const QSize& textureImageSize, bool alphaPremultiply);
   ~AtlasGroup();

    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /*! Returns group name. */
    inline const QString& name() const { return m_name; }
    /*! Returns image object. */
    inline QImage& image() { return m_image; }
    /* Clears image. */
    void clearImage();
    /*! Returns root data path. */
    inline const QString& root() const { return m_root; }
    /*! Returns texture image name. */
    inline const QString& textureImageName() const { return m_textureImageName; }
    /*! Returns texture format name. */
    inline const QString& textureFormatName() const { return m_textureFormatName; }
    /*! Returns texture filters name. */
    inline const QString& textureFiltersName() const { return m_textureFiltersName; }
    /* Adds entry to pool. */
    void addEntry(AtlasGroupEntry* entry);
    /*! Returns list of entries. */
    inline QList<AtlasGroupEntry*>& entries() { return m_entries; }
    /*! Returns atlas texture image size (in pixels). */
    inline const QSize& textureImageSize() const { return m_textureImageSize; }
    /*! Returns alpha premultiplication flag. */
    inline bool isAlphaPremultiplied() const { return m_alphaPremultiply; }

  private:

    /*! Name. */
    QString m_name;
    /*! Data root path. */
    QString m_root;
    /*! Texture image name (relative to root) without extension. */
    QString m_textureImageName;
    /*! Texture filters name. */
    QString m_textureFiltersName;
    /*! List of group entries. */
    QList<AtlasGroupEntry*> m_entries;
    /*! Atlas image. */
    QImage m_image;
    /*! Atlas texture image size (in pixels). */
    QSize m_textureImageSize;
    /*! Atlas texture format name. */
    QString m_textureFormatName;
    /*! Alpha premultiplication flag. */
    bool m_alphaPremultiply;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // ATLAS_GROUP_H