#include "SwfFile.h"
#include "SwfHeader.h"
#include "SwfTag.h"
#include "SwfPlaceObject2Tag.h"
#include "SwfDefineShapeTag.h"
#include "SwfRemoveObjectTag.h"
#include "SwfRemoveObject2Tag.h"
#include "SwfDefineBitsJpeg3Tag.h"
#include "SwfDefineBitsLosslessTag.h"
#include "SwfDefineBitsTag.h"
#include "SwfParser.h"
#include "ResourceManager.h"
#include <QFile>
#include <QDataStream>
#include <QDebug>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
struct DisplayData
{
  DisplayData() : characterId(0), color(Qt::white) {}

  quint16 characterId;
  Matrix matrix;
  QColor color;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SwfFile::SwfFile(const QString& animationName, float scale, const QMap<QString, QString>& sequences, QObject* parent) : QObject(parent),
                                                                                                                        m_scale(scale),
                                                                                                                        m_animationName(animationName),
                                                                                                                        m_sequences(sequences)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SwfFile::~SwfFile()
{
  qDeleteAll(m_tags);
  m_tags.clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Process the file with a given name. */
bool SwfFile::process(const QString &fileName)
{
  SwfDataStream* stream;

  qDebug() << "Processing" << fileName << "...";

  // create and read header
  m_header = new SwfHeader();
  if (NULL == (stream = m_header->process(fileName)))
  {
    // error!
    return false;
  }

  // read rest of the file (tags)
  do
  {
    // process tag
    SwfTag* tag = SwfTag::ProcessTag(*stream, this);
    if (tag)
    {
      // add to pool
      m_tags.append(tag);
    }

  } while ((QDataStream::Ok == stream->status()) && ! stream->atEnd());

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Serializes into EGE XML. */
bool SwfFile::serialize(QXmlStreamWriter& stream)
{
  stream.writeStartElement("imaged-animation");
  stream.writeAttribute("name", m_animationName);
  stream.writeAttribute("fps", QString::number(m_header->fps()));
  stream.writeAttribute("size", QString("%1 %2").arg(m_header->frameSize().width() * m_scale).arg(m_header->frameSize().height() * m_scale));

  // serialize objects section
  if ( ! serializeObjectsSection(stream))
  {
    // error!
    return false;
  }

  // serialize sequences section
  if ( ! serializeSequencesSection(stream))
  {
    // error!
    return false;
  }

  // serialize frames
  if ( ! serializeFrames(stream))
  {
    // error!
    return false;
  }

  return ! stream.hasError();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns dictionary. */
Dictionary& SwfFile::dictionary()
{
  return m_dictionary;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Serializes objects section into EGE XML. */
bool SwfFile::serializeObjectsSection(QXmlStreamWriter& stream)
{
  ResourceManager* resourceManager = qobject_cast<SwfParser*>(parent())->resourceManager();

  // find all shapes (objects referred by frames)
  foreach (const SwfTag* tag, m_tags)
  {
    if ((SWF_TAG_ID_DEFINE_SHAPE == tag->id()) || (SWF_TAG_ID_DEFINE_SHAPE_2 == tag->id()))
    {
      const SwfDefineShapeTag* shapeTag = qobject_cast<const SwfDefineShapeTag*>(tag);

      stream.writeStartElement("object");
      stream.writeAttribute("id", QString::number(shapeTag->characterId()));

      // go thru all fill styles
      foreach (const FillStyle& style, shapeTag->shapeStyle().fillStyles)
      {
        // check if bitmap-based style
        if (((FST_REPEAT_BITMAP == style.type) || (FST_CLIPPED_BITMAP == style.type) || (FST_NONSMOOTHED_REPEAT_BITMAP == style.type) ||
            (FST_NONSMOOTHED_CLIPPED_BITMAP == style.type)) && (0xffff != style.bitmapCharacterId))
        {
          QImage image;

          // locate bitmap object in dictionary
          QObject* bitmapObject = m_dictionary.value(style.bitmapCharacterId, NULL);
          if (NULL == bitmapObject)
          {
            // error!
            qCritical() << "Could not locate bitmap tag in dictionary!";
            return false;
          }

          SwfDefineBitsJpeg3Tag* jpeg3        = qobject_cast<SwfDefineBitsJpeg3Tag*>(bitmapObject);
          SwfDefineBitsLosslessTag* lossless  = qobject_cast<SwfDefineBitsLosslessTag*>(bitmapObject);
          SwfDefineBitsTag* jpeg              = qobject_cast<SwfDefineBitsTag*>(bitmapObject);

          int imageId = -1;

          if (NULL != jpeg3)
          {
            imageId = jpeg3->imageId();
          }
          else if (NULL != lossless)
          {
            imageId = lossless->imageId();
          }
          else if (NULL != jpeg)
          {
            imageId = jpeg->imageId();
          }

          image = resourceManager->image(imageId);
          if (image.isNull())
          {
            // error!
            qCritical() << "Could not find image referenced by characterID" << style.bitmapCharacterId << "for animation" << m_animationName;
            return false;
          }

          stream.writeStartElement("child");

          stream.writeAttribute("material", resourceManager->generateNameFromImageId(imageId));
          stream.writeAttribute("translate", QString("%1 %2").arg(style.bitmapMatrix.translateX * m_scale).arg(style.bitmapMatrix.translateY * m_scale));
          stream.writeAttribute("scale", QString("%1 %2").arg(style.bitmapMatrix.scaleX).arg(style.bitmapMatrix.scaleY));
          stream.writeAttribute("skew", QString("%1 %2").arg(style.bitmapMatrix.rotX).arg(style.bitmapMatrix.rotY));
       //   stream.writeAttribute("color", QString("%1 %2 %3 %4").arg(data.color.redF()).arg(data.color.greenF()).arg(data.color.blueF()).arg(data.color.alphaF()));
          stream.writeAttribute("size", QString("%1 %2").arg(image.width() * m_scale).arg(image.height() * m_scale));

          stream.writeEndElement();
        }
      }

      stream.writeEndElement();
    }
  }

  return ! stream.hasError();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Serializes sequences section into EGE XML. */
bool SwfFile::serializeSequencesSection(QXmlStreamWriter& stream)
{
  // add default playback sequence
  stream.writeStartElement("sequence");
  stream.writeAttribute("name", "all");

  QString sequencerFrames;
  for (int i = 0; i < m_header->frameCount(); ++i)
  {
    sequencerFrames += QString::number(i) + " ";
  }
  stream.writeAttribute("frames", sequencerFrames.trimmed());
  stream.writeEndElement();
  
  // store external sequences
  for (QMap<QString, QString>::const_iterator it = m_sequences.begin(); it != m_sequences.end(); ++it)
  {
    stream.writeStartElement("sequence");
    stream.writeAttribute("name", it.key());
    stream.writeAttribute("frames", it.value());
    stream.writeEndElement();
  }

  return ! stream.hasError();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Serializes frames into EGE XML. */
bool SwfFile::serializeFrames(QXmlStreamWriter& stream)
{
  // display list accessed by depth
  QMap<quint16, DisplayData> displayList;

  // go thru all tags
  quint16 frameCount = 0;
  foreach (const SwfTag* tag, m_tags)
  {
    if (SWF_TAG_ID_PLACE_OBJECT_2 == tag->id())
    {
      const SwfPlaceObject2Tag* placeTag = qobject_cast<const SwfPlaceObject2Tag*>(tag);

      // add or modify character at given depth
      DisplayData& data = displayList[placeTag->depth()];
      if (placeTag->hasCharacterId())
      {
        data.characterId = placeTag->characterId();
      }

      if (placeTag->hasMatrix())
      {
        data.matrix = placeTag->matrix();
      }

      if (placeTag->hasColorTransformation())
      {
        // TAGE - possibly this should be taken from object
        const QColor baseColor(Qt::white);
        const QColor& addColor  = placeTag->colorTransformation().addTerms;
        const QColor& multColor = placeTag->colorTransformation().multTerms;

        data.color.setRed(qMax(0, qMin(static_cast<int>((baseColor.red() * multColor.redF()) + addColor.red()), 255)));
        data.color.setGreen(qMax(0, qMin(static_cast<int>((baseColor.green() * multColor.greenF()) + addColor.green()), 255)));
        data.color.setBlue(qMax(0, qMin(static_cast<int>((baseColor.blue() * multColor.blueF()) + addColor.blue()), 255)));
        data.color.setAlpha(qMax(0, qMin(static_cast<int>((baseColor.alpha() * multColor.alphaF()) + addColor.alpha()), 255)));
      }
    }
    else if (SWF_TAG_ID_REMOVE_OBJECT == tag->id())
    {
      const SwfRemoveObjectTag* removeTag = qobject_cast<const SwfRemoveObjectTag*>(tag);

      // remove given character at given depth
      if (displayList.contains(removeTag->depth()) && displayList[removeTag->depth()].characterId == removeTag->characterId())
      {
        // remove character at given depth
        displayList.remove(removeTag->depth());
      }
      else
      {
        qWarning() << "Could not remove character" << removeTag->characterId() << "at depth" << removeTag->depth() << "No such character and/or depth!";
      }
    }
    else if (SWF_TAG_ID_REMOVE_OBJECT_2 == tag->id())
    {
      const SwfRemoveObject2Tag* removeTag = qobject_cast<const SwfRemoveObject2Tag*>(tag);

      // remove character at given depth
      displayList.remove(removeTag->depth());
    }
    else if (SWF_TAG_ID_SHOW_FRAME == tag->id())
    {
      stream.writeStartElement("frame");

      for (QMap<quint16, DisplayData>::const_iterator it = displayList.constBegin(); it != displayList.constEnd(); ++it)
      {
      //  const quint16& depth = it.key();
        const DisplayData& data = it.value();

        stream.writeStartElement("action");

        stream.writeAttribute("object-id", QString::number(data.characterId));
      //  stream.writeAttribute("queue", QString::number(depth));
        stream.writeAttribute("translate", QString("%1 %2").arg(data.matrix.translateX * m_scale).arg(data.matrix.translateY * m_scale));
        stream.writeAttribute("scale", QString("%1 %2").arg(data.matrix.scaleX).arg(data.matrix.scaleY));
        stream.writeAttribute("skew", QString("%1 %2").arg(data.matrix.rotX).arg(data.matrix.rotY));
        stream.writeAttribute("color", QString("%1 %2 %3 %4").arg(data.color.redF()).arg(data.color.greenF()).arg(data.color.blueF()).arg(data.color.alphaF()));

        stream.writeEndElement();
      }

      stream.writeEndElement();

      // increment frame count
      ++frameCount;
    }
  }

  stream.writeEndElement();

  // check if processed correct number of frames
  if (frameCount != m_header->frameCount())
  {
    // error!
    qCritical() << "Frame count does not match!" << m_animationName << "Found" << frameCount << "expected" << m_header->frameCount();
    return false;
  }

  return ! stream.hasError();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
