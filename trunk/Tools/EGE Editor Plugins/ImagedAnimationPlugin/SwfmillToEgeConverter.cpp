#include "SwfmillToEgeConverter.h"

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
 *  @param input   SWFMILL generated input XML stream
 *  @param output  EGE framework output XML stream
 */
bool SwfMillToEgeConverter::convert(QXmlStreamReader& input, QXmlStreamWriter& output)
{
  // process children
  while (!input.atEnd() && !output.hasError())
  {
    QXmlStreamReader::TokenType token = input.readNext();
    switch (token)
    {
      case QXmlStreamReader::StartElement:

        if ("Header" == input.name())
        {
          processHeaderTag(input);
        }
        else if ("DefineBitsJPEG3" == input.name())
        {
          processDefineBitsJPEG3Tag(input);
        }
        else if ("DefineShape" == input.name())
        {
          processDefineShapeTag(input);
        }
        else if ("PlaceObject2" == input.name())
        {
          processPlaceObject2Tag(input);
        }
        else if ("ShowFrame" == input.name())
        {
          processShowFrameTag(input);
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

  m_fps         = input.attributes().value("framerate").toString().toInt(&ok);
  m_frameCount  = input.attributes().value("frames").toString().toInt(&ok);

  return !input.hasError() && ok;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Processes SWFMILL DefineBitsJPEG3 tag. */
bool SwfMillToEgeConverter::processDefineBitsJPEG3Tag(QXmlStreamReader& input)
{
  bool ok = true;

  ObjectData objectData;
  objectData.objectId = input.attributes().value("objectID").toString().toInt(&ok);
  
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
          objectData.data = QByteArray::fromBase64(input.text().toAscii());
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

  m_objects.append(objectData);

  return !input.hasError() && ok;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Processes SWFMILL DefineShape tag. */
bool SwfMillToEgeConverter::processDefineShapeTag(QXmlStreamReader& input)
{
  bool ok = true;

  ShapeData shapeData;
  shapeData.referenceObjectId = 0;
  shapeData.objectId = input.attributes().value("objectID").toString().toInt(&ok);
  shapeData.translate = QVector2D(0, 0);
  shapeData.scale     = QVector2D(1, 1);
  shapeData.skew      = QVector2D(0, 0);

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
          shapeData.referenceObjectId = input.attributes().value("objectID").toString().toInt(&ok);
        }
        else if ("Transform" == input.name())
        {
          processTransformTag(input, shapeData.translate, shapeData.scale, shapeData.skew);
        }
        break;

      case QXmlStreamReader::EndElement:

        if ("DefineShape" == input.name())
        {
          // done
          done = true;
        }
        break;
    }
  }

  Q_ASSERT(0 < shapeData.referenceObjectId);
  m_shapes.append(shapeData);

  return !input.hasError() && ok;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Processes SWFMILL PlaceObject2 tag. */
bool SwfMillToEgeConverter::processPlaceObject2Tag(QXmlStreamReader& input)
{
  bool ok = true;

  FrameData frameData;

  frameData.objectId  = input.attributes().value("objectID").toString().toInt(&ok);
  frameData.depth     = input.attributes().value("depth").toString().toInt(&ok);
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
          processTransformTag(input, frameData.translate, frameData.scale, frameData.skew);
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

  return !input.hasError() && ok;
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

  if (input.attributes().hasAttribute("transX"))
  {
    translate.setX(input.attributes().value("transX").toString().toInt(&ok));
  }

  if (input.attributes().hasAttribute("transY"))
  {
    translate.setY(input.attributes().value("transY").toString().toInt(&ok));
  }

  if (input.attributes().hasAttribute("scaleX"))
  {
    scale.setX(input.attributes().value("scaleX").toString().toFloat(&ok));
  }

  if (input.attributes().hasAttribute("scaleY"))
  {
    scale.setY(input.attributes().value("scaleY").toString().toFloat(&ok));
  }

  if (input.attributes().hasAttribute("skewX"))
  {
    skew.setX(input.attributes().value("skewX").toString().toFloat(&ok));
  }

  if (input.attributes().hasAttribute("skewY"))
  {
    skew.setY(input.attributes().value("skewY").toString().toFloat(&ok));
  }

  return !input.hasError() && ok;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Generates EGE compilant XML. */
bool SwfMillToEgeConverter::generateEgeXML(QXmlStreamWriter& output)
{
  output.writeStartElement("imaged-animation");

  // main attributes
  output.writeAttribute("name", "my-name");
  output.writeAttribute("fps", QString::number(m_fps));

  // shapes
  foreach (const ShapeData& shapeData, m_shapes)
  {
    output.writeStartElement("object");
    
    output.writeAttribute("name", QString("%1").arg(shapeData.objectId));
    output.writeAttribute("material", "my-material");
    output.writeAttribute("translate", QString("%1 %2").arg(shapeData.translate.x()).arg(shapeData.translate.y()));
    output.writeAttribute("scale", QString("%1 %2").arg(shapeData.scale.x()).arg(shapeData.scale.y()));
    output.writeAttribute("skew", QString("%1 %2").arg(shapeData.skew.x()).arg(shapeData.skew.y()));

    output.writeEndElement();
  }

  // frames
  foreach (const FrameDataList& frameDataList, m_framesDataList)
  {
    output.writeStartElement("frame");
    
    foreach (const FrameData& frameData, frameDataList)
    {
      output.writeStartElement("action");

      output.writeAttribute("object", QString("%1").arg(frameData.objectId));
      output.writeAttribute("queue", QString("%1").arg(frameData.depth));
      output.writeAttribute("translate", QString("%1 %2").arg(frameData.translate.x()).arg(frameData.translate.y()));
      output.writeAttribute("scale", QString("%1 %2").arg(frameData.scale.x()).arg(frameData.scale.y()));
      output.writeAttribute("skew", QString("%1 %2").arg(frameData.skew.x()).arg(frameData.skew.y()));

      output.writeEndElement();
    }

    output.writeEndElement();
  }

  output.writeEndElement();

  return !output.hasError();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
