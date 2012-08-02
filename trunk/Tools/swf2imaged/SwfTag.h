#ifndef SWF_TAG_H
#define SWF_TAG_H

#include "SwfDataStream.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define SWF_TAG_ID_END                          (0)
#define SWF_TAG_ID_SHOW_FRAME                   (1)
#define SWF_TAG_ID_DEFINE_SHAPE                 (2)
#define SWF_TAG_ID_REMOVE_OBJECT                (5)
#define SWF_TAG_ID_DEFINE_BITS                  (6)
#define SWF_TAG_ID_JPEG_TABLES                  (8)
#define SWF_TAG_ID_SET_BACKGROUND_COLOR         (9)
#define SWF_TAG_ID_DEFINE_BITS_LOSSLESS         (20)
#define SWF_TAG_ID_DEFINE_SHAPE_2               (22)
#define SWF_TAG_ID_PLACE_OBJECT_2               (26)
#define SWF_TAG_ID_REMOVE_OBJECT_2              (28)
#define SWF_TAG_ID_DEFINE_BITS_JPEG3            (35)
#define SWF_TAG_ID_DEFINE_BITS_LOSSLESS_2       (36)
#define SWF_TAG_ID_DEFINE_SPRITE                (39)
#define SWF_TAG_ID_FILE_ATTRIBUTE               (69)
#define SWF_TAG_ID_META_DATA                    (77)
#define SWF_TAG_ID_DEFINE_SCENE_AND_LABEL_DATA  (86)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class SwfFile;
class ResourceManager;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! @brief Class representing base of SWF tag. */
class SwfTag : public QObject
{
  Q_OBJECT

  public:

    SwfTag();
    virtual ~SwfTag();

  public:

    /* Creates SWFTag of a proper type. */
    static SwfTag* ProcessTag(SwfDataStream& data, SwfFile* file);

  public:

    /*! Reads data from file. */
    virtual bool read(SwfDataStream& data) = 0;
    /*! Returns ID. */
    inline quint16 id() const { return m_id; }
    /*! Returns length (in bytes). */
    inline quint32 length() const { return m_length; }
    /*! Returns parent file. */
    inline SwfFile* file() const { return m_file; }

  protected:

    /* Returns resource manager. */
    ResourceManager* resourceManager() const;

  private:

    /*! Tag ID. */
    quint16 m_id;
    /*! Tag length (in bytes). */
    quint32 m_length;
    /*! Parent file object. */
    SwfFile* m_file;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // SWF_TAG_H
