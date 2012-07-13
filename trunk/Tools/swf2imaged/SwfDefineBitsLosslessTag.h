#ifndef SWF_DEFINEBITSLOSSLESS_TAG_H
#define SWF_DEFINEBITSLOSSLESS_TAG_H

#include "SwfTag.h"
#include <QByteArray>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! @brief Class representing DefineBitsLossless SWF tag. */
class SwfDefineBitsLosslessTag : public SwfTag
{
  Q_OBJECT

  public:

    SwfDefineBitsLosslessTag();
   ~SwfDefineBitsLosslessTag();

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

#endif // SWF_DEFINEBITSLOSSLESS_TAG_H
