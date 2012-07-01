#include <QImage>
#include <QXmlStreamWriter>
#include <QFile>
#include <QTextStream>
#include <QRegExp>
#include <QTimer>
#include <QDebug>
#include "FontDataGenerator.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define VERSION 0.1
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
FontDataGenerator::FontDataGenerator(int argc, char *argv[]) : QApplication(argc, argv),
                                                               m_fontName("dummy-font-name"),
                                                               m_materialName("dummy-material-name"),
                                                               m_borderColor(QColor(255, 0, 255, 255))
{
  // process command-line
  QStringList args = QCoreApplication::arguments();
  for (int i = 0; i < args.size(); )
  {
    // check if input image file switch
    if (("--ifile" == args[i]) && (i + 1 < args.size()))
    {
      m_imageFileName = args[i + 1];
      i += 2;
    }
    // check if input character data file switch
    else if (("--cfile" == args[i]) && (i + 1 < args.size()))
    {
      m_charsFileName = args[i + 1];
      i += 2;
    }
    // check if output file switch
    else if (("--o" == args[i]) && (i + 1 < args.size()))
    {
      m_outputFileName = args[i + 1];
      i += 2;
    }
    // check if border color switch
    else if (("--bcolor" == args[i]) && (i + 1 < args.size()))
    {
      QString colorString = args[i + 1];

      bool ok;
      uint colorValue = colorString.toUInt(&ok, 16);
      if (ok)
      {
        m_borderColor = QColor(colorValue >> 24, (colorValue & 0x00ff0000) >> 16, (colorValue & 0x0000ff00) >> 8, colorValue & 0x000000ff);
      }

      i += 2;
    }
    // check if font name switch
    else if (("--name" == args[i]) && (i + 1 < args.size()))
    {
      m_fontName = args[i + 1];
      i += 2;
    }
    // check if material name switch
    else if (("--material" == args[i]) && (i + 1 < args.size()))
    {
      m_materialName = args[i + 1];
      i += 2;
    }
    else
    {
      // go to next
      ++i;
    }
  }          

  // schedule processing
  QTimer::singleShot(100, this, SLOT(onStart()));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
FontDataGenerator::~FontDataGenerator()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Starts process. */
bool FontDataGenerator::process()
{
  qDebug() << "Processing" << m_fontName << "...";

  // load font image
  QImage image(m_imageFileName);
  if (image.isNull())
  {
    // error!
    qDebug() << "ERROR: Could not open image file:" << m_imageFileName;
    return false;
  }

  QFile charsFile(m_charsFileName);
  if ( ! charsFile.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    // error!
    qDebug() << "ERROR: Could not open characters definition file:" << m_charsFileName;
    return false;
  }

  QTextStream in(&charsFile);
  QString chars = in.readAll();
  chars.remove(QRegExp("[\n\a]"));

  // create the bit array for fast lookup for processed pixels
  QBitArray processedPixelArray(image.width() * image.height());

  // scan the image
  for (int y = 0; y < image.height(); ++y)
  {
    for (int x = 0; x < image.width(); ++x)
    {
      // check if border color
      QRgb pixelColor = image.pixel(x, y);
      if (pixelColor != m_borderColor.rgba())
      {
        // check if NOT already processed
        if ( ! processedPixelArray.at(x + y * image.width()))
        {
          // process region
          processRegion(x, y, image, processedPixelArray);
        }
      }
    }
  }

  // open XML output stream
  QString output;
  QXmlStreamWriter stream(&output);
  stream.setAutoFormatting(true);
  stream.setAutoFormattingIndent(2);

  // generate XML output
  stream.writeStartDocument();
  stream.writeStartElement("font");
  stream.writeAttribute("name", m_fontName);
  stream.writeAttribute("material", m_materialName);

  int i = 0;
  foreach(const RegionData& regionData, m_regionList)
  {
    stream.writeStartElement("glyph");

    int character = (i < chars.length()) ? chars.at(i).unicode() : -1;
    if (-1 == character)
    {
      qDebug() << "WARNING: No definition for character at index" << i << "found!";
    }

    stream.writeAttribute("value", QString::number(character));
    stream.writeAttribute("width", QString::number(regionData.rect.width()));
    stream.writeAttribute("image-x", QString::number(static_cast<qreal>(regionData.rect.x()) / image.width()));
    stream.writeAttribute("image-y", QString::number(static_cast<qreal>(regionData.rect.y()) / image.height()));
    stream.writeAttribute("image-width", QString::number(static_cast<qreal>(regionData.rect.width()) / image.width()));
    stream.writeAttribute("image-height", QString::number(static_cast<qreal>(regionData.rect.height()) / image.height()));

    stream.writeEndElement();

    ++i;
  }

  stream.writeEndElement();
  stream.writeEndDocument();

  // check for errors
  if (stream.hasError())
  {
    // error!
    qDebug() << "ERROR: XML generation error!!";
    return false;
  }

  // store output to the file
  QFile outputFile(m_outputFileName);
  if ( ! outputFile.open(QIODevice::WriteOnly))
  {
    // error!
    qDebug() << "ERROR: Could not open output file:" << m_outputFileName;
    return false;
  }

  QTextStream out(&outputFile);
  out << output;
  outputFile.close();

  qDebug() << "SUCCESS";
  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Processes the region.
    @param   startX          Left position of the region to process within font image (in pixels).
    @param   startY          Top position of the region to process within font image (in pixels).
    @param   image           Image on which region is to be processed.
    @param   processedPixels Bit array containing bits already processed.
    @note    Method determines the extent of the region based on boarder color. Region is then added into glyphs list.
 */
void FontDataGenerator::processRegion(int startX, int startY, const QImage& image, QBitArray& processedPixels)
{
  RegionData regionData;
  regionData.rect = QRect(startX, startY, 1, 1);

  // scan the image
  for (int y = startY; y < image.height(); ++y)
  {
    // check if we are below the region already
    QRgb pixelColor = image.pixel(startX, y);
    if (pixelColor == m_borderColor.rgba())
    {
      // done
      break;
    }

    for (int x = startX; x < image.width(); ++x)
    {
      // check if border color
      QRgb pixelColor = image.pixel(x, y);
      if (pixelColor != m_borderColor.rgba())
      {
        // mark processed
        processedPixels.setBit(x + y * image.width());

        regionData.rect.setWidth(x - startX + 1);
        regionData.rect.setHeight(y - startY + 1);
      }
      else
      {
        // end of region reached
        break;
      }
    }
  }

  // add to pool
  m_regionList.append(regionData);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called to begin processing. */
void FontDataGenerator::onStart()
{
  // show header
  printHeader();

  // check if required data is missing
  if (m_imageFileName.isEmpty() || m_outputFileName.isEmpty())
  {
    // error!
    printSyntax();
    exit(1);
    return;
  }
  
  // process data
  if (!process())
  {
    // error!
    exit(2);
    return;
  }

  // done
  exit(0);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Prints syntax to standard output. */
void FontDataGenerator::printSyntax() const
{
  qDebug() << "Usage syntax:";
  qDebug() << "fontgen --ifile <filename> [--cfile <filename>] --o <filename> [--bcolor RRGGBBAA] [--name <name>] [--material <name>]";
  qDebug() << "";
  qDebug() << "--ifile      Full path to font image file.";
  qDebug() << "--cfile      [Optional] Full path to text file containing font characters sequence represented by image.";
  qDebug() << "--o          Full path to output XML file.";
  qDebug() << "--bcolor     [Optional] RGBA border color. This is color which seperates characters within image. Default: Magenta FF00FFFF.";
  qDebug() << "--name       [Optional] Font name.";
  qDebug() << "--material   [Optional] Material name.";
  qDebug() << "";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Prints header to standard output. */
void FontDataGenerator::printHeader() const
{
  QString version;
  version.setNum(VERSION, 'f', 2);
  QByteArray versionData = version.toAscii();

  qDebug() << "";
  qDebug() << "Font Data Generator, version" << version.toAscii();
  qDebug() << "Albert Banaszkiewicz, Little Bee Studios Ltd., 2011-2012";
  qDebug() << "";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
