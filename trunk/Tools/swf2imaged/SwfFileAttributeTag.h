#ifndef SWF_FILEATTRIBUTE_TAG_H
#define SWF_FILEATTRIBUTE_TAG_H

#include "SwfTag.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! @brief Class representing FileAttribute SWF tag. */
class SwfFileAttributeTag : public SwfTag
{
  public:

    SwfFileAttributeTag();
   ~SwfFileAttributeTag();

  private:

    /* SwfTag override. Reads data from file. */
    bool read(SwfDataStream& data) override;

  private:

    /*! TRUE if SWF file uses hardware acceleration to blit graphics if possible. */
    bool m_useDirectBlit; 
    /*! TRUE if SWF file uses GPU compositing if possible. */
    bool m_useGpu;        
    /*! TRUE if Metadata TAG if present. */
    bool m_hasMetadata;  
    /*! TRUE if SWF uses ActionScript 3.0. Otherwise, uses ActionScript 1.0 or 2.0. */
    bool m_actionScript;  
    /*! TRUE if network access is given to the file. */
    bool m_useNetwork;    
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // SWF_FILEATTRIBUTE_TAG_H