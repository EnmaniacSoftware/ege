#ifndef SWF_DEFINEBITS_TAG_H
#define SWF_DEFINEBITS_TAG_H

#include "SwfTag.h"
#include <QByteArray>
#include <QImage>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! @brief Class representing DefineBits SWF tag. */
class SwfDefineBitsTag : public SwfTag
{
  Q_OBJECT

  public:

    SwfDefineBitsTag();
   ~SwfDefineBitsTag();

    /* Returns character ID. */
    quint16 characterId() const;
    /* Returns image ID. */
    int imageId() const;

  private:

    /* SwfTag override. Reads data from file. */
    bool read(SwfDataStream& data) override;

  private:

    /*! Character Id. */
    quint16 m_characterId;
    /*! Compressed image data buffer. */
    QByteArray m_imageData;
    /*! Image id. */
    int m_imageId;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // SWF_DEFINEBITS_TAG_H
