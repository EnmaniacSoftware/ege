#include "SwfFile.h"
#include "SwfHeader.h"
#include "SwfTag.h"
#include "SwfPlaceObject2Tag.h"
#include "SwfDefineShapeTag.h"
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
SwfFile::SwfFile(float scale, QObject* parent) : QObject(parent),
                                                 m_scale(scale)
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
  qDebug() << "Processing" << fileName << "...";

  // store file name (without extension)
  m_name = fileName.section("/", -1);
  m_name = m_name.section(".", 0, 0);

  // open input file
  QFile file(fileName);
  if ( ! file.open(QIODevice::ReadOnly))
  {
    // error!
    qCritical() << "Unable to open file" << fileName;
    return false;
  }

  // prepare data steam
  SwfDataStream input(&file);

  // create and read header
  m_header = new SwfHeader();
  if ( ! m_header->read(input))
  {
    // error!
    return false;
  }

  // read rest of the file (tags)
  do
  {
    // process tag
    SwfTag* tag = SwfTag::ProcessTag(input, this);
    if (tag)
    {
      // add to pool
      m_tags.append(tag);
    }

  } while ((QDataStream::Ok == input.status()) && !input.atEnd());

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Serializes into EGE XML. */
bool SwfFile::serialize(QXmlStreamWriter& stream)
{
  stream.writeStartElement("imaged-animation");
  stream.writeAttribute("name", m_name);
  stream.writeAttribute("fps", QString::number(m_header->fps()));
  stream.writeAttribute("size", QString("%1 %2").arg(m_header->frameSize().width()).arg(m_header->frameSize().height()));

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
    if (SWF_TAG_ID_DEFINE_SHAPE == tag->id())
    {
      const SwfDefineShapeTag* shapeTag = qobject_cast<const SwfDefineShapeTag*>(tag);

      // go thru all fill styles
      foreach (const FillStyle& style, shapeTag->shapeStyle().fillStyles)
      {
        // check if bitmap-based style
        if (((FST_REPEAT_BITMAP == style.type) || (FST_CLIPPED_BITMAP == style.type) || (FST_NONSMOOTHED_REPEAT_BITMAP == style.type) ||
            (FST_NONSMOOTHED_CLIPPED_BITMAP == style.type)) && (0xffff != style.bitmapCharacterId))
        {
          const QImage image = resourceManager->image(style.bitmapCharacterId);
          if (image.isNull())
          {
            // error!
            qCritical() << "Could not find image referenced by characterID" << style.bitmapCharacterId;
            return false;
          }

          stream.writeStartElement("object");

          stream.writeAttribute("id", QString::number(shapeTag->characterId()));
          stream.writeAttribute("material", resourceManager->generateNameFromCharacterId(style.bitmapCharacterId));
//          stream.writeAttribute("translate", QString("%1 %2").arg(data.matrix.translateX).arg(data.matrix.translateY));
//          stream.writeAttribute("scale", QString("%1 %2").arg(data.matrix.scaleX).arg(data.matrix.scaleY));
//          stream.writeAttribute("skew", QString("%1 %2").arg(data.matrix.rotX).arg(data.matrix.rotY));
//          stream.writeAttribute("color", QString("%1 %2 %3 %4").arg(data.color.redF()).arg(data.color.greenF()).arg(data.color.blueF()).arg(data.color.alphaF()));
          stream.writeAttribute("size", QString("%1 %2").arg(image.width()).arg(image.height()));

          stream.writeEndElement();
        }
      }
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
        Q_ASSERT("Implement!");
      }
    }
    else if (SWF_TAG_ID_SHOW_FRAME == tag->id())
    {
      stream.writeStartElement("frame");

      for (QMap<quint16, DisplayData>::const_iterator it = displayList.constBegin(); it != displayList.constEnd(); ++it)
      {
        const quint16& depth = it.key();
        const DisplayData& data = it.value();

        stream.writeStartElement("action");

        stream.writeAttribute("object-id", QString::number(data.characterId));
        stream.writeAttribute("queue", QString::number(depth));
        stream.writeAttribute("translate", QString("%1 %2").arg(data.matrix.translateX).arg(data.matrix.translateY));
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
    qCritical() << "Frame count does not match!";
    return false;
  }

  return ! stream.hasError();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
