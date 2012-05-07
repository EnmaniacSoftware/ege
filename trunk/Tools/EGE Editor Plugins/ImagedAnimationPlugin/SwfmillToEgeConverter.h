#ifndef SWFMILLTOEGECONVERTER_H
#define SWFMILLTOEGECONVERTER_H

#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QList>
#include <QVector2D>
#include <QMap>
#include <QRectF>
#include <QImage>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class SwfMillToEgeConverter
{
  public:

    SwfMillToEgeConverter();
    ~SwfMillToEgeConverter();

    /* Converts SWFMILL   generated XML into EGE framework format. 
     * @param input       SWFMILL generated input XML stream.
     * @param output      EGE framework output XML stream.
     * @param baseName    Base name for generated assets.
     * @param scaleFactor Scale factor to be applied to transformations and other size related figures.
     */
    bool convert(QXmlStreamReader& input, QXmlStreamWriter& output, const QString& baseName, float scaleFactor = 1.0f);

  private:

    /*! Available object actions. */
    enum ObjectAction
    {
      OA_PLACE = 0,
      OA_REMOVE
    };

    /*! Frame data structure. */
    struct FrameData // rename to FrameActionData
    {
      int depth;                              /*!< Depth at which action is to be done. */
      int objectId;                           /*!< Object ID (shape) for which action is to be done. */
      ObjectAction action;                    /*!< Action to be done. */
      QVector2D translate;
      QVector2D scale;
      QVector2D skew;
    };

    /*! Object data structure. */
    struct ObjectData
    {
      int objectId;                           /*!< Object ID. */
      QImage image;                           /*!< Image. */
    };

    /*! Shape object data structure. */
    struct ShapeObjectData
    {
      int objectId;                           /*!< ID of the referenced object. */
      int depth;                              /*!< Current depth object is place on. Only used while generating EGE XML. */
      QVector2D translate;                    /*!< Base translation vector. */
      QVector2D scale;                        /*!< Base scale vector. */
      QVector2D skew;                         /*!< Base skew vector. */
    };

    /*! Shape data structure. */
    struct ShapeData
    {
      int objectId;                             /*!< Object ID (shape object). */
      QRectF boundingBox;                       /*!< Bounding box. */
      QList<ShapeObjectData> shapeDataObjects;  /*!< Underlying objects the shape is built of. */
    };

    typedef QList<FrameData> FrameDataList;
    typedef QList<ObjectData> ObjectDataList;
    typedef QList<ShapeData> ShapeDataList;

  private:

    /* Processes SWFMILL Header tag. */
    bool processHeaderTag(QXmlStreamReader& input);
    /* Processes SWFMILL DefineBitsJPEG3 tag. */
    bool processDefineBitsJPEG3Tag(QXmlStreamReader& input);
    /* Processes SWFMILL DefineBitsLossless2 tag. */
    bool processDefineBitsLossless2Tag(QXmlStreamReader& input);
    /* Processes SWFMILL DefineShape tag. */
    bool processDefineShapeTag(QXmlStreamReader& input);
    /* Processes SWFMILL DefineShape2 tag. */
    bool processDefineShape2Tag(QXmlStreamReader& input);
    /* Processes SWFMILL PlaceObject2 tag. */
    bool processPlaceObject2Tag(QXmlStreamReader& input);
    /* Processes SWFMILL RemoveObject2 tag. */
    bool processRemoveObject2Tag(QXmlStreamReader& input);
    /* Processes SWFMILL ShowFrame tag. */
    bool processShowFrameTag(QXmlStreamReader& input);
    /* Processes SWFMILL Transform tag. */
    bool processTransformTag(QXmlStreamReader& input, QVector2D& translate, QVector2D& scale, QVector2D& skew) const;
    /* Processes SWFMILL Rectangle tag. */
    bool processRectangleTag(QXmlStreamReader& input, QRectF& rect) const;
    /* Generates EGE compilant XML. */
    bool generateEgeXML(QXmlStreamWriter& output);
    /* Returns given action name. */
    QString objectActionName(ObjectAction action) const;
    /* Clears converter data. */
    void clear();
    /* Checks whether object with given ID exists. */
    bool objectExists(int objectId) const;

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
    /*! Map containing current depth for objects [depth, objectId]. */
    QMap<int, int> m_objectPlacementMap;
    /*! Base name. */
    QString m_baseName;
    /*! Current scale factor. */
    float m_scaleFactor;
    /*! Display rect. */
    QRectF m_displayRect;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // SWFMILLTOEGECONVERTER_H