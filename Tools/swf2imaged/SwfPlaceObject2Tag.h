#ifndef SWF_PLACEOBJECT2_TAG_H
#define SWF_PLACEOBJECT2_TAG_H

#include "SwfTag.h"
#include "SwfTypes.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! @brief Class representing PlaceObject2 SWF tag. */
class SwfPlaceObject2Tag : public SwfTag
{
  Q_OBJECT

  public:

    SwfPlaceObject2Tag();
   ~SwfPlaceObject2Tag();

    /* Returns depth character should be placed on. */
    quint16 depth() const;
    /* Returns character id. */
    quint16 characterId() const;
    /* Returns transformation matrix. */
    const Matrix& matrix() const;
    /* Returns color transformation. */
    const ColorTransform& colorTransformation() const;
    /* Returns TRUE if transformation matrix is defined. */
    bool hasMatrix() const;
    /* Returns TRUE if transformation matrix is defined. */
    bool hasColorTransformation() const;
    /* Returns TRUE if character ID is defined. */
    bool hasCharacterId() const;
    /* Returns TRUE if move flag is defined. */
    bool shallMove() const;

  private:

    /* SwfTag override. Reads data from file. */
    bool read(SwfDataStream& data) override;

  private:

    /*! Depth. */
    quint16 m_depth;
    /*! Character Id. */
    quint16 m_characterId;
    /*! Matrix. */
    Matrix m_matrix;
    /*! Ratio. */
    quint16 m_ratio;
    /*! Clip depth. */
    quint16 m_clipDepth;
    /*! Color transformation. */
    ColorTransform m_colorTransformation;
    /*! TRUE if clip action are defined. */
    bool m_hasClipActions;
    /*! TRUE if clip depth is defined. */
    bool m_hasClipDepth;
    /*! TRUE if character name is defined. */
    bool m_hasName;
    /*! TRUE if ratio is defined. */
    bool m_hasRatio;
    /*! TRUE if color transformation is defined. */
    bool m_hasColorTransform;
    /*! TRUE if transformation matrix is defined. */
    bool m_hasMatrix;
    /*! TRUE if character ID is defined. */
    bool m_hasCharacter;
    /*! TRUE if character is to be moved rather than added. */
    bool m_move;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // SWF_PLACEOBJECT2_TAG_H
