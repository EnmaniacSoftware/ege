#include "SwfmillToEgeConverter.h"
#include <QByteArray>
#include <QDebug>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define SWF_TWIPS_TO_PIXELS(x) ((x) * 0.05f)
#define RETURN_FALSE_IF_ERROR(condition) if (!(condition)) return false
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SwfMillToEgeConverter::SwfMillToEgeConverter()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SwfMillToEgeConverter::~SwfMillToEgeConverter()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Converts SWFMILL generated XML into EGE framework format. 
 *  @param input      SWFMILL generated input XML stream.
 *  @param output     EGE framework output XML stream.
 *  @param baseName   Base name for generated assets.
 */
bool SwfMillToEgeConverter::convert(QXmlStreamReader& input, QXmlStreamWriter& output, const QString& baseName)
{
  bool ok;

  // clean up
  clear();

  // store base name
  m_baseName = baseName;

  // process children
  while (!input.atEnd() && !output.hasError())
  {
    QXmlStreamReader::TokenType token = input.readNext();
    switch (token)
    {
      case QXmlStreamReader::StartElement:

        qDebug() << "Tag found:" << input.name();

        if ("Header" == input.name())
        {
          ok = processHeaderTag(input);
          RETURN_FALSE_IF_ERROR(ok);
        }
        else if ("DefineBitsJPEG3" == input.name())
        {
          ok = processDefineBitsJPEG3Tag(input);
          RETURN_FALSE_IF_ERROR(ok);
        }
        else if ("DefineBitsLossless2" == input.name())
        {
          ok = processDefineBitsLossless2Tag(input);
          RETURN_FALSE_IF_ERROR(ok);
        }
        else if ("DefineShape" == input.name())
        {
          ok = processDefineShapeTag(input);
          RETURN_FALSE_IF_ERROR(ok);
        }
        else if ("DefineShape2" == input.name())
        {
          ok = processDefineShape2Tag(input);
          RETURN_FALSE_IF_ERROR(ok);
        }
        else if ("PlaceObject2" == input.name())
        {
          ok = processPlaceObject2Tag(input);
          RETURN_FALSE_IF_ERROR(ok);
        }
        else if ("RemoveObject2" == input.name())
        {
          ok = processRemoveObject2Tag(input);
          RETURN_FALSE_IF_ERROR(ok);
        }
        else if ("ShowFrame" == input.name())
        {
          ok = processShowFrameTag(input);
          RETURN_FALSE_IF_ERROR(ok);
        }
        else
        {
          qDebug() << "Skipping.";
        }
        break;

      case QXmlStreamReader::EndElement:

        // done
        break;
    }
  }

  // TAGE
  generateEgeXML(output);

  return !input.hasError() && !output.hasError();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Processes SWFMILL Header tag. */
bool SwfMillToEgeConverter::processHeaderTag(QXmlStreamReader& input)
{
  bool ok = true;

  m_fps = input.attributes().value("framerate").toString().toInt(&ok);
  RETURN_FALSE_IF_ERROR(ok);

  m_frameCount = input.attributes().value("frames").toString().toInt(&ok);
  RETURN_FALSE_IF_ERROR(ok);

  return !input.hasError();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Processes SWFMILL DefineBitsJPEG3 tag. */
bool SwfMillToEgeConverter::processDefineBitsJPEG3Tag(QXmlStreamReader& input)
{
  bool ok = true;

  ObjectData objectData;
  objectData.objectId = input.attributes().value("objectID").toString().toInt(&ok);
  RETURN_FALSE_IF_ERROR(ok);

  int offsetToAlpha = input.attributes().value("offset_to_alpha").toString().toInt(&ok);
  RETURN_FALSE_IF_ERROR(ok);

  // process children
  bool done = false;
  while (!input.atEnd() && !done)
  {
    QXmlStreamReader::TokenType token = input.readNext();
    switch (token)
    {
      case QXmlStreamReader::Characters:

        if (!input.isWhitespace())
        {
          // retrive all image data
          // NOTE: this may contain alpha block as well
          QByteArray imageData = QByteArray::fromBase64(input.text().toAscii());

          // retrieve alpha block
          QByteArray alphaData = imageData.mid(offsetToAlpha);

          // remove alpha block from pixel data
          imageData = imageData.left(offsetToAlpha);

          // create image from pixel data
          QImage image = QImage::fromData(imageData);

          // prepend alpha data block with uncompressed size so qUncompress can be used
          // NOTE: alpha block format is 8-bit per pixel
          int alphaBlockUncompressedSize = objectData.image.width() * objectData.image.height();

          alphaData.prepend((alphaBlockUncompressedSize & 0x000000ff));
          alphaData.prepend((alphaBlockUncompressedSize & 0x0000ff00) >> 8);
          alphaData.prepend((alphaBlockUncompressedSize & 0x00ff0000) >> 16);
          alphaData.prepend((alphaBlockUncompressedSize & 0xff000000) >> 24);

          // decompress alpha channel
          alphaData = qUncompress(alphaData);

          // create ARGBA image from pixel data image
          objectData.image = image.convertToFormat(QImage::Format_ARGB32);
      
          // TAGE - optimize

          // add alpha channel
          for (int y = 0; y < objectData.image.height(); ++y)
          {
            for (int x = 0; x < objectData.image.width(); ++x)
            {
              QRgb color = objectData.image.pixel(x, y);
              objectData.image.setPixel(x, y, qRgba(qRed(color), qGreen(color), qBlue(color), alphaData[x + y * objectData.image.width()]));
            }
          }
        }
        break;

      case QXmlStreamReader::EndElement:

        if ("DefineBitsJPEG3" == input.name())
        {
          // done
          done = true;
        }
        break;
    }
  }

  // add to pool
  m_objects.append(objectData);

  return !input.hasError();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Processes SWFMILL DefineBitsLossless2 tag. */
bool SwfMillToEgeConverter::processDefineBitsLossless2Tag(QXmlStreamReader& input)
{
  bool ok = true;

  ObjectData objectData;
  objectData.objectId = input.attributes().value("objectID").toString().toInt(&ok);
  RETURN_FALSE_IF_ERROR(ok); 

  int format = input.attributes().value("format").toString().toInt(&ok);
  RETURN_FALSE_IF_ERROR(ok);

  int width = input.attributes().value("width").toString().toInt(&ok);
  RETURN_FALSE_IF_ERROR(ok);

  int height = input.attributes().value("height").toString().toInt(&ok);
  RETURN_FALSE_IF_ERROR(ok);

  Q_ASSERT(5 == format);

  // process children
  bool done = false;
  while (!input.atEnd() && !done)
  {
    QXmlStreamReader::TokenType token = input.readNext();
    switch (token)
    {
      case QXmlStreamReader::Characters:

        if (!input.isWhitespace())
        {
          QByteArray imageData = QByteArray::fromBase64(input.text().toAscii());
          
          // prepend pixel data block with uncompressed size so qUncompress can be used
          // NOTE: image rows need to be 4-bytes aligned
          int imageUncompressedSize = width * height;

          imageData.prepend((imageUncompressedSize & 0x000000ff));
          imageData.prepend((imageUncompressedSize & 0x0000ff00) >> 8);
          imageData.prepend((imageUncompressedSize & 0x00ff0000) >> 16);
          imageData.prepend((imageUncompressedSize & 0xff000000) >> 24);

          // decompress image
          imageData = qUncompress(imageData);

          // create empty ARGB image
          objectData.image = QImage(width, height, QImage::Format_ARGB32);

          // TAGE - optimize

          // fill in pixel data
          for (int y = 0; y < height; ++y)
          {
            for (int x = 0; x < width; ++x)
            {
              objectData.image.setPixel(x, y, qRgba(imageData[4 * (x + y * width) + 1], imageData[4 * (x + y * width) + 2], 
                                                    imageData[4 * (x + y * width) + 3], imageData[4 * (x + y * width) + 0]));
            }
          }
        }
        break;

      case QXmlStreamReader::EndElement:

        if ("DefineBitsLossless2" == input.name())
        {
          // done
          done = true;
        }
        break;
    }
  }

  // add to pool
  m_objects.append(objectData);

  return !input.hasError();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Processes SWFMILL DefineShape tag. */
bool SwfMillToEgeConverter::processDefineShapeTag(QXmlStreamReader& input)
{
  bool ok = true;

  ShapeData shapeData;
  shapeData.objectId    = input.attributes().value("objectID").toString().toInt(&ok);
  RETURN_FALSE_IF_ERROR(ok);

  shapeData.boundingBox = QRectF();

  // process children
  bool done = false;
  while (!input.atEnd() && !done)
  {
    QXmlStreamReader::TokenType token = input.readNext();
    switch (token)
    {
      case QXmlStreamReader::StartElement:

        if ("ClippedBitmap2" == input.name())
        {
          ShapeObjectData shapeObject;
          shapeObject.objectId = input.attributes().value("objectID").toString().toInt(&ok);
          RETURN_FALSE_IF_ERROR(ok);
        
          shapeData.shapeDataObjects << shapeObject;
        }
        else if ("Transform" == input.name())
        {
          ShapeObjectData& shapeObject = shapeData.shapeDataObjects.last();
          ok = processTransformTag(input, shapeObject.translate, shapeObject.scale, shapeObject.skew);
          RETURN_FALSE_IF_ERROR(ok);

          // rescale
          // TAGE - scale seems always to be multiplied by 20 for some reason
          shapeObject.scale *= 0.05f;
        }
        else if ("Rectangle" == input.name())
        {
          ok = processRectangleTag(input, shapeData.boundingBox);
          RETURN_FALSE_IF_ERROR(ok);
        }
        break;

      case QXmlStreamReader::EndElement:

        if ("DefineShape" == input.name())
        {
          // done
          done = true;
        }
        else if ("ClippedBitmap2" == input.name())
        {
          // check if improper shape
          ShapeObjectData& shapeObject = shapeData.shapeDataObjects.last();
          if (0xffff == shapeObject.objectId)
          {
            // pop it off the pool
            shapeData.shapeDataObjects.pop_back();
          }
        }
        break;
    }
  }

  // add to pool
  m_shapes.append(shapeData);

  return !input.hasError();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Processes SWFMILL DefineShape2 tag. */
bool SwfMillToEgeConverter::processDefineShape2Tag(QXmlStreamReader& input)
{
  bool ok = true;

  ShapeData shapeData;
  shapeData.objectId = input.attributes().value("objectID").toString().toInt(&ok);
  RETURN_FALSE_IF_ERROR(ok);

  shapeData.boundingBox = QRectF();

  // process children
  bool done = false;
  while (!input.atEnd() && !done)
  {
    QXmlStreamReader::TokenType token = input.readNext();
    switch (token)
    {
      case QXmlStreamReader::StartElement:

        if ("ClippedBitmap2" == input.name())
        {
          ShapeObjectData shapeObject;
          shapeObject.objectId = input.attributes().value("objectID").toString().toInt(&ok);
          RETURN_FALSE_IF_ERROR(ok);

          shapeData.shapeDataObjects << shapeObject;
        }
        else if ("Transform" == input.name())
        {
          ShapeObjectData& shapeObject = shapeData.shapeDataObjects.last();
          ok = processTransformTag(input, shapeObject.translate, shapeObject.scale, shapeObject.skew);
          RETURN_FALSE_IF_ERROR(ok);

          // rescale
          // TAGE - scale seems always to be multiplied by 20 for some reason
          shapeObject.scale *= 0.05f;
        }
        else if ("Rectangle" == input.name())
        {
          ok = processRectangleTag(input, shapeData.boundingBox);
          RETURN_FALSE_IF_ERROR(ok);
        }
        break;

      case QXmlStreamReader::EndElement:

        if ("DefineShape2" == input.name())
        {
          // done
          done = true;
        }
        else if ("ClippedBitmap2" == input.name())
        {
          // check if improper shape
          ShapeObjectData& shapeObject = shapeData.shapeDataObjects.last();
          if (0xffff == shapeObject.objectId)
          {
            // pop it off the pool
            shapeData.shapeDataObjects.pop_back();
          }
        }
        break;
    }
  }

  // add to pool
  m_shapes.append(shapeData);

  return !input.hasError();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Processes SWFMILL PlaceObject2 tag. */
bool SwfMillToEgeConverter::processPlaceObject2Tag(QXmlStreamReader& input)
{
  bool ok = true;

  FrameData frameData;
  frameData.action = OA_PLACE;

  frameData.depth = input.attributes().value("depth").toString().toInt(&ok);
  RETURN_FALSE_IF_ERROR(ok);

  frameData.objectId = input.attributes().value("objectID").toString().toInt(&ok);
  if (!ok)
  {
    // try to find if placement map
    frameData.objectId = m_objectPlacementMap.value(frameData.depth, 0);
  }

  // check if object for given depth not found
  if (0 == frameData.objectId)
  {
    // error!
    return false;
  }

  // update object placement map
  m_objectPlacementMap.insert(frameData.depth, frameData.objectId);

  frameData.translate = QVector2D(0, 0);
  frameData.scale     = QVector2D(1, 1);
  frameData.skew      = QVector2D(0, 0);
//  frameData.depth     = input.attributes().value("replace").toString().toInt(&error);

  // process children
  bool done = false;
  while (!input.atEnd() && !done)
  {
    QXmlStreamReader::TokenType token = input.readNext();
    switch (token)
    {
      case QXmlStreamReader::StartElement:

        if ("Transform" == input.name())
        {
          ok = processTransformTag(input, frameData.translate, frameData.scale, frameData.skew);
          RETURN_FALSE_IF_ERROR(ok);
        }
        break;

      case QXmlStreamReader::EndElement:

        if ("PlaceObject2" == input.name())
        {
          // done
          done = true;
        }
        break;
    }
  }

  // add to current frame data list
  m_currentFrameDataList.append(frameData);

  return !input.hasError();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Processes SWFMILL RemoveObject2 tag. */
bool SwfMillToEgeConverter::processRemoveObject2Tag(QXmlStreamReader& input)
{
  bool ok = true;

  FrameData frameData;
  frameData.action = OA_REMOVE;

  frameData.depth = input.attributes().value("depth").toString().toInt(&ok);
  RETURN_FALSE_IF_ERROR(ok);

  frameData.objectId = input.attributes().value("objectID").toString().toInt(&ok);
  if (!ok)
  {
    // try to find if placement map
    frameData.objectId = m_objectPlacementMap.value(frameData.depth, 0);
  }

  // check if object for given depth not found
  if (0 == frameData.objectId)
  {
    // error!
    return false;
  }

  // add to current frame data list
  m_currentFrameDataList.append(frameData);

  return !input.hasError();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Processes SWFMILL ShowFrame tag. */
bool SwfMillToEgeConverter::processShowFrameTag(QXmlStreamReader& input)
{
  // add all pending frame datas into pool (if any)
  if (!m_currentFrameDataList.isEmpty())
  {
    m_framesDataList.append(m_currentFrameDataList);

    // clean up current frame
    m_currentFrameDataList.clear();
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Processes SWFMILL Transform tag. */
bool SwfMillToEgeConverter::processTransformTag(QXmlStreamReader& input, QVector2D& translate, QVector2D& scale, QVector2D& skew) const
{
  bool ok = true;

  if (input.attributes().hasAttribute("transX") && ok)
  {
    translate.setX(qRound(SWF_TWIPS_TO_PIXELS(input.attributes().value("transX").toString().toInt(&ok))));
    RETURN_FALSE_IF_ERROR(ok);
  }

  if (input.attributes().hasAttribute("transY") && ok)
  {
    translate.setY(qRound(SWF_TWIPS_TO_PIXELS(input.attributes().value("transY").toString().toInt(&ok))));
    RETURN_FALSE_IF_ERROR(ok);
  }

  if (input.attributes().hasAttribute("scaleX") && ok)
  {
    scale.setX(input.attributes().value("scaleX").toString().toFloat(&ok));
    RETURN_FALSE_IF_ERROR(ok);
  }

  if (input.attributes().hasAttribute("scaleY") && ok)
  {
    scale.setY(input.attributes().value("scaleY").toString().toFloat(&ok));
    RETURN_FALSE_IF_ERROR(ok);
  }

  if (input.attributes().hasAttribute("skewX") && ok)
  {
    skew.setX(input.attributes().value("skewX").toString().toFloat(&ok));
    RETURN_FALSE_IF_ERROR(ok);
  }

  if (input.attributes().hasAttribute("skewY") && ok)
  {
    skew.setY(input.attributes().value("skewY").toString().toFloat(&ok));
    RETURN_FALSE_IF_ERROR(ok);
  }

  return !input.hasError();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Processes SWFMILL Rectangle tag. */
bool SwfMillToEgeConverter::processRectangleTag(QXmlStreamReader& input, QRectF& rect) const
{
  bool ok = true;

  if (input.attributes().hasAttribute("left"))
  {
    rect.setLeft(SWF_TWIPS_TO_PIXELS(input.attributes().value("left").toString().toFloat(&ok)));
    RETURN_FALSE_IF_ERROR(ok);
  }

  if (input.attributes().hasAttribute("right") && ok)
  {
    rect.setRight(SWF_TWIPS_TO_PIXELS(input.attributes().value("right").toString().toInt(&ok)));
    RETURN_FALSE_IF_ERROR(ok);
  }

  if (input.attributes().hasAttribute("top") && ok)
  {
    rect.setTop(SWF_TWIPS_TO_PIXELS(input.attributes().value("top").toString().toFloat(&ok)));
    RETURN_FALSE_IF_ERROR(ok);
  }

  if (input.attributes().hasAttribute("bottom") && ok)
  {
    rect.setBottom(SWF_TWIPS_TO_PIXELS(input.attributes().value("bottom").toString().toInt(&ok)));
    RETURN_FALSE_IF_ERROR(ok);
  }

  return !input.hasError();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Generates EGE compilant XML. */
bool SwfMillToEgeConverter::generateEgeXML(QXmlStreamWriter& output)
{
  // materials
  foreach (const ObjectData& object, m_objects)
  {
    output.writeStartElement("material");

    output.writeAttribute("name", QString("%1_object_%2").arg(m_baseName).arg(object.objectId));
    output.writeAttribute("src-blend", "src-alpha");
    output.writeAttribute("dst-blend", "one-minus-src-alpha");

    output.writeStartElement("texture");
    output.writeAttribute("name", QString("%1_object_%2").arg(m_baseName).arg(object.objectId));
    output.writeEndElement();

    output.writeEndElement();

    // store image
    if (!object.image.save(QString("%1_object_%2.png").arg(m_baseName).arg(object.objectId), "png"))
    {
      // error!
      return false;
    }
  }

  output.writeStartElement("imaged-animation");

  // main attributes
  output.writeAttribute("name", m_baseName);
  float duration = 0.0f;
  if (0 < m_fps)
  {
    duration = (1.0f / m_fps) * m_framesDataList.count();
  }
  output.writeAttribute("duration", QString::number(duration));

  // shapes
  QList<int> processedObjectIds;
  foreach (const ShapeData& shapeData, m_shapes)
  {
    foreach (const ShapeObjectData& shapeObject, shapeData.shapeDataObjects)
    {
      // check if object wasnt saved yet
      if (!processedObjectIds.contains(shapeObject.objectId))
      {
        output.writeStartElement("object");
    
        // found object data referenced by current shape object
        ObjectData objectData;
        for (int i = 0; i < m_objects.count(); ++i)
        {
          objectData = m_objects[i];
          if (objectData.objectId == shapeObject.objectId)
          {
            // found
            break;
          }
        }

        output.writeAttribute("id", QString("%1").arg(shapeObject.objectId));
        output.writeAttribute("material", QString("%1_object_%2").arg(m_baseName).arg(objectData.objectId));
        output.writeAttribute("translate", QString("%1 %2").arg(shapeObject.translate.x()).arg(shapeObject.translate.y()));
        output.writeAttribute("scale", QString("%1 %2").arg(shapeObject.scale.x()).arg(shapeObject.scale.y()));
        output.writeAttribute("skew", QString("%1 %2").arg(shapeObject.skew.x()).arg(shapeObject.skew.y()));
        output.writeAttribute("rect", QString("%1 %2 %3 %4").arg(shapeData.boundingBox.x()).arg(shapeData.boundingBox.y()).arg(shapeData.boundingBox.width()).arg(shapeData.boundingBox.height()));
        output.writeAttribute("size", QString("%1 %2").arg(objectData.image.width()).arg(objectData.image.height()));
   
        output.writeEndElement();

        // add to pool of already saved object
        processedObjectIds << shapeObject.objectId;
      }
    }
  }

  // frames
  foreach (const FrameDataList& frameDataList, m_framesDataList)
  {
    output.writeStartElement("frame");
    
    foreach (const FrameData& frameData, frameDataList)
    {
      // find shape
      foreach (const ShapeData& shapeData, m_shapes)
      {
        // check if proper shape
        if (shapeData.objectId == frameData.objectId)
        {
          // go thru all shape objects
          foreach (const ShapeObjectData& shapeObject, shapeData.shapeDataObjects)
          {
            output.writeStartElement("action");

            output.writeAttribute("type", objectActionName(frameData.action));
            output.writeAttribute("object-id", QString("%1").arg(shapeObject.objectId));
            output.writeAttribute("queue", QString("%1").arg(frameData.depth));
            output.writeAttribute("translate", QString("%1 %2").arg(frameData.translate.x()).arg(frameData.translate.y()));
            output.writeAttribute("scale", QString("%1 %2").arg(frameData.scale.x()).arg(frameData.scale.y()));
            output.writeAttribute("skew", QString("%1 %2").arg(frameData.skew.x()).arg(frameData.skew.y()));

            output.writeEndElement();
          }
        }
      }
    }

    output.writeEndElement();
  }

  output.writeEndElement();

  return !output.hasError();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns given action name. */
QString SwfMillToEgeConverter::objectActionName(ObjectAction action) const
{
  switch (action)
  {
    case OA_PLACE:  return "place";
    case OA_REMOVE: return "remove";
  }

  Q_ASSERT("Invalid action");
  return "";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Clears converter data. */
void SwfMillToEgeConverter::clear()
{
  m_framesDataList.clear();
  m_currentFrameDataList.clear();
  m_objects.clear();
  m_shapes.clear();
  m_objectPlacementMap.clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
