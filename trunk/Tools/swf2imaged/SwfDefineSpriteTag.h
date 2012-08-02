#ifndef SWFDEFINESPRITETAG_H
#define SWFDEFINESPRITETAG_H

#include "SwfTag.h"
#include "DisplayList.h"
#include <QList>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! @brief Class representing DefineSprite SWF tag. */
class SwfDefineSpriteTag : public SwfTag
{
  Q_OBJECT

  public:

    SwfDefineSpriteTag();
   ~SwfDefineSpriteTag();

    /* Returns character ID. */
    quint16 characterId() const;
    /* Updates display data. */
    void update(DisplayList::Data& displayData);

  private:

    /* SwfTag override. Reads data from file. */
    bool read(SwfDataStream& data) override;
    /* Returns the list of tags for a given frame index. */
    QList<SwfTag*> frameTags(quint16 frameIndex) const;

  protected:

    /*! Character Id. */
    quint16 m_characterId;
    /*! Frame count. */
    quint16 m_frameCount;
    /*! List of all valid tags. */
    QList<SwfTag*> m_tags;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // SWFDEFINESPRITETAG_H
