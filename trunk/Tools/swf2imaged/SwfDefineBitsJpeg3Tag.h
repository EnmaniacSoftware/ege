#ifndef SWF_DEFINEBITSJPEG3_TAG_H
#define SWF_DEFINEBITSJPEG3_TAG_H

#include "SwfTag.h"
#include <QByteArray>
#include <QImage>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! @brief Class representing DefineBitsJpeg3 SWF tag. */
class SwfDefineBitsJpeg3Tag : public SwfTag
{
  Q_OBJECT

  public:

    SwfDefineBitsJpeg3Tag();
   ~SwfDefineBitsJpeg3Tag();

    /* Returns character ID. */
    quint16 characterId() const;

  private:

    /* SwfTag override. Reads data from file. */
    bool read(SwfDataStream& data) override;

  private:

    /*! Character Id. */
    quint16 m_characterId;
    /*! Alpha data offset (in bytes). */
    quint32 m_alphaOffset;
    /*! Compressed image data buffer. */
    QByteArray m_imageData;
    /*! Alpha data buffer. */
    QByteArray m_alphaData;
    /*! Image id. */
    int m_imageId;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // SWF_DEFINEBITSJPEG3_TAG_H
