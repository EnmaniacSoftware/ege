#ifndef SWFMILLTOEGECONVERTER_H
#define SWFMILLTOEGECONVERTER_H

#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QList>
#include <QVector2D>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class SwfMillToEgeConverter
{
  public:

    SwfMillToEgeConverter();
    ~SwfMillToEgeConverter();

    /* Converts SWFMILL generated XML into EGE framework format. 
     * @param input   SWFMILL generated input XML stream
     * @param output  EGE framework output XML stream
     */
    bool convert(QXmlStreamReader& input, QXmlStreamWriter& output);

  private:

    /* Processes SWFMILL Header tag. */
    bool processHeaderTag(QXmlStreamReader& input);
    /* Processes SWFMILL DefineBitsJPEG3 tag. */
    bool processDefineBitsJPEG3Tag(QXmlStreamReader& input);
    /* Processes SWFMILL DefineShape tag. */
    bool processDefineShapeTag(QXmlStreamReader& input);
    /* Processes SWFMILL PlaceObject2 tag. */
    bool processPlaceObject2Tag(QXmlStreamReader& input);
    /* Processes SWFMILL ShowFrame tag. */
    bool processShowFrameTag(QXmlStreamReader& input);
    /* Processes SWFMILL Transform tag. */
    bool processTransformTag(QXmlStreamReader& input, QVector2D& translate, QVector2D& scale, QVector2D& skew) const;
    /* Generates EGE compilant XML. */
    bool generateEgeXML(QXmlStreamWriter& output);

  private:

    enum Action
    {
      ACTION_PLACE = 0,
    };

    /*! Frame data structure. */
    struct FrameData
    {
      int depth;                              /*!< Depth at which action is to be done. */
      int objectId;                           /*!< Object . */
      QVector2D translate;
      QVector2D scale;
      QVector2D skew;
    };

    /*! Object data structure. */
    struct ObjectData
    {
      int objectId;                           /*!< Object ID. */
    };

    /*! Shape data structure. */
    struct ShapeData
    {
      int objectId;                           /*!< Object ID. */
      int referenceObjectId;                  /*!< ID of the object which is refered by shape. */
      QVector2D translate;                    /*!< Base translation vector. */
      QVector2D scale;                        /*!< Base scale vector. */
      QVector2D skew;                         /*!< Base skew vector. */
    };

    typedef QList<FrameData> FrameDataList;
    typedef QList<ObjectData> ObjectDataList;
    typedef QList<ShapeData> ShapeDataList;

  private:

    /*! Animation FPS. */
    int m_fps;
    /*! Animation frame count. */
    int m_frameCount;
    /*! List of frame data lists. Each frame data list consists of action to be performed in a single frame. */
    QList<FrameDataList> m_framesDataList;
    /*! Current frame data list. */
    FrameDataList m_currentFrameDataList;
    /*! List of all objects. */
    ObjectDataList m_objects;
    /*! List of all shapes. */
    ShapeDataList m_shapes;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // SWFMILLTOEGECONVERTER_H