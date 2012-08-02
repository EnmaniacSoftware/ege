#include "TextureAtlasGenerator.h"
#include "AtlasGroupEntry.h"
#include <QStringList>
#include <QDebug>
#include <QTimer>
#include <QFile>
#include <QPainter>
#include <QProcess>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
struct TextureFormatData
{
  QString textureFormatName;
  QString textureFileExtension;
  TextureAtlasGenerator::TextureFormat textureFormat;
};

static TextureFormatData l_textureFormatData[] = {  { "rgba", ".png", TextureAtlasGenerator::TF_RGBA8888 },
                                                    { "rgb", ".png", TextureAtlasGenerator::TF_RGB888 },
                                                    { "rgba5551_pvrtc", ".pvr", TextureAtlasGenerator::TF_PVRTC_RGBA5551 },
                                                    { "rgba4444_pvrtc", ".pvr", TextureAtlasGenerator::TF_PVRTC_RGBA4444 },
                                                    { "2bpp_pvrtc", ".pvr", TextureAtlasGenerator::TF_PVRTC_2BPP },
                                                    { "4bpp_pvrtc", ".pvr", TextureAtlasGenerator::TF_PVRTC_4BPP },
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define VERSION 1.01f
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function for sorting object from greatest area to smallest. */
static bool SortGreaterArea(AtlasGroupEntry* left, AtlasGroupEntry* right)
{
  return left->image().width() * left->image().height() > right->image().width() * right->image().height();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function for sorting object from greatest width to smallest. */
static bool SortGreaterWidth(AtlasGroupEntry* left, AtlasGroupEntry* right)
{
  return left->image().width() > right->image().width();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Local function for sorting object from greatest height to smallest. */
static bool SortGreaterHeight(AtlasGroupEntry* left, AtlasGroupEntry* right)
{
  return left->image().height() > right->image().height();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Maps texture format name into extension name. */
static QString GetTextureFileExtension(const QString& formatName)
{
  // go thru all supported texture formats
  for (int i = 0; i < sizeof (l_textureFormatData) / sizeof (l_textureFormatData[0]); ++i)
  {
    const TextureFormatData& data = l_textureFormatData[i];
    if (data.textureFormatName == formatName)
    {
      // found
      return data.textureFileExtension;
    }
  }

  // default
  Q_ASSERT(true);
  return ".png";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Gets texture format value from texture format name. */
static TextureAtlasGenerator::TextureFormat MapImageFormat(const QString& formatName)
{
  // go thru all supported texture formats
  for (int i = 0; i < sizeof (l_textureFormatData) / sizeof (l_textureFormatData[0]); ++i)
  {
    const TextureFormatData& data = l_textureFormatData[i];
    if (data.textureFormatName == formatName)
    {
      // found
      return data.textureFormat;
    }
  }

  // default
  Q_ASSERT(true);
  return TextureAtlasGenerator::TF_RGBA8888;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextureAtlasGenerator::TextureAtlasGenerator(int argc, char* argv[]) : QApplication(argc, argv),
                                                                       m_sortMethod(SM_NONE)
{
  // process command-line
  QStringList args = QCoreApplication::arguments();
  for (int i = 0; i < args.size(); )
  {
    // check if input fileswitch
    if (("--input" == args[i]) && (i + 1 < args.size()))
    {
      m_inputDataFilePath = args[i + 1];
      i += 2;
    }
    // check if output file switch
    else if (("--output" == args[i]) && (i + 1 < args.size()))
    {
      m_outputXmlPath = args[i + 1];
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
TextureAtlasGenerator::~TextureAtlasGenerator()
{
  // go thru all groups
  for (QList<AtlasGroup*>::iterator it = m_groups.begin(); it != m_groups.end();)
  {
    AtlasGroup* group = *it;

    if (NULL != group)
    {
      delete group;
    }

    m_groups.erase(it++);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Prints syntax. */
void TextureAtlasGenerator::printSyntax() const
{
  qDebug() << "Usage syntax:";
  qDebug() << "tatlasgen --input <filepath> --output <filepath>";
  qDebug() << "";
  qDebug() << "--input      path to XML input file containing.";
  qDebug() << "--output     output XML file path.";
  qDebug() << "";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Prints application info header. */
void TextureAtlasGenerator::printHeader() const
{
  QString version;
  version.setNum(VERSION, 'f', 2);
  QByteArray versionData = version.toAscii();

  qDebug() << "";
  qDebug() << "Texture Atlas Generator, version" << versionData.data();
  qDebug() << "Albert Banaszkiewicz, Little Bee Studios Ltd., 2011-2012";
  qDebug() << "";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Processes all data. */
bool TextureAtlasGenerator::process()
{
  qDebug() << "Processing input file:" << m_inputDataFilePath;

  // open input XML file
  QFile inputXml(m_inputDataFilePath);
  if ( ! inputXml.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    // error!
    qDebug() << "ERROR: Could not open input XML file:" << m_inputDataFilePath;
    return false;
  }

  QXmlStreamReader input(&inputXml);

  // process children
  bool done = false;
  while (!input.atEnd() && !done)
  {
    QXmlStreamReader::TokenType token = input.readNext();
    switch (token)
    {
      case QXmlStreamReader::StartElement:

        if ("atlas-group" == input.name())
        {
          if ( ! processAtlasGroupTag(input))
          {
            // error!
            return false;
          }
        }
        break;

      case QXmlStreamReader::EndElement:

        if ("atlas-group" == input.name())
        {
          // done
          done = true;
        }
        break;
    }
  }

  // generate atlases
  return generateAll();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Generates atlases. */
bool TextureAtlasGenerator::generateAll()
{
  // open XML output stream
  QDomDocument doc;
  QDomElement root = doc.createElement("resources");
  doc.appendChild(root);

  // process all groups one by one
  for (QList<AtlasGroup*>::iterator it = m_groups.begin(); it != m_groups.end();)
  {
    AtlasGroup* group = *it;

    qDebug() << "Processing" << group->name() << "...";

    // reset sort mode
    m_sortMethod = SM_NONE;

    // generate atlas for current group
    if ( ! generate(doc, root, group))
    {
      qCritical() << "ERROR: Could not generate atlas for group: " << group->name();
      return false;
    }

    // remove group
    m_groups.erase(it++);
  }

  // store output to the file
  QFile outputFile(outputXmlPath());
  if ( ! outputFile.open(QIODevice::WriteOnly))
  {
    // error!
    qDebug() << "ERROR: Could not open output file:" << outputXmlPath();
    return false;
  }

  QTextStream out(&outputFile);
  out << doc.toString();
  outputFile.close();

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Generates atlas for given group. */
bool TextureAtlasGenerator::generate(QDomDocument& doc, QDomElement& root, AtlasGroup* group)
{
  Q_ASSERT(group);

  // prepare image
  group->clearImage();

  // select next sort methods
  switch (sortMethod())
  {
    case SM_NONE:           m_sortMethod = SM_GREATER_AREA;   break;
    case SM_GREATER_AREA:   m_sortMethod = SM_GREATER_WIDTH;  break;
    case SM_GREATER_WIDTH:  m_sortMethod = SM_GREATER_HEIGHT; break;

    default:

      // if we reach here means all methods failed
      return false;
  }

  // get sorted entries for current group
  switch (sortMethod())
  {
    case SM_GREATER_AREA:   qSort(group->entries().begin(), group->entries().end(), SortGreaterArea); break;
    case SM_GREATER_WIDTH:  qSort(group->entries().begin(), group->entries().end(), SortGreaterWidth); break;
    case SM_GREATER_HEIGHT: qSort(group->entries().begin(), group->entries().end(), SortGreaterHeight); break;
  }

  // create atlas tree root node
  AtlasNode* nodeRoot = new AtlasNode();
  if (NULL == nodeRoot)
  {
    // error!
    qDebug() << "ERROR: Could not create atlas tree root node!";
    return false;
  }

  // set root node to max size
  nodeRoot->m_rect = QRect(0, 0, group->textureImageSize().width(), group->textureImageSize().height());
  
  // create group element
  QDomElement groupElement = doc.createElement("group");
  groupElement.setAttribute("name", QString("atlas-%1").arg(group->name()));

  // create texture element
  QDomElement textureElement = doc.createElement("texture");
  groupElement.appendChild(textureElement);

  textureElement.setAttribute("type", "2d");
  textureElement.setAttribute("name", QString("atlas-%1").arg(group->name()));
  textureElement.setAttribute("path", group->textureImageName() + GetTextureFileExtension(group->textureFormatName()));
  textureElement.setAttribute("mag-filter", group->textureFiltersName());
  textureElement.setAttribute("min-filter", group->textureFiltersName());
  textureElement.setAttribute("alpha-premultiplied", group->isAlphaPremultiplied() ? "true" : "false");

  // go thru all elements belonging to current group
  for (QList<AtlasGroupEntry*>::const_iterator it = group->entries().begin(); it != group->entries().end(); ++it)
  {
    AtlasGroupEntry* entry = *it;

    // try to find node
    AtlasNode* node = nodeRoot->insert(entry);
    if (NULL == node)
    {
      // error!
      delete nodeRoot;

      // try again with another sort method
      return generate(doc, root, group);
    }

    // store group entry for which place was found
    node->m_entry = entry;    

    // create XML corresponding node 
    QDomElement textureImageElement = doc.createElement("texture-image");
    groupElement.appendChild(textureImageElement);
    textureImageElement.setAttribute("name", entry->name());
    textureImageElement.setAttribute("texture", QString("atlas-%1").arg(group->name()));
    
    // calculate real (without spacing) rect occupied by element
    QRect rect(node->m_rect.x() + node->m_entry->spacing().x(), node->m_rect.y() + node->m_entry->spacing().y(), 
               node->m_rect.width() - node->m_entry->spacing().x() - node->m_entry->spacing().z(), 
               node->m_rect.height() - node->m_entry->spacing().y() - node->m_entry->spacing().w());

#ifdef _DEBUG
    textureImageElement.setAttribute("pixel-rect", QString("%1 %2 %3 %4").arg(rect.x()).arg(rect.y()).arg(rect.width()).arg(rect.height()));
#endif _DEBUG
    textureImageElement.setAttribute("rect", QString("%1 %2 %3 %4").arg(rect.x() * 1.0f / group->textureImageSize().width()).
                                                                    arg(rect.y() * 1.0f / group->textureImageSize().height()).
                                                                    arg(rect.width() * 1.0f / group->textureImageSize().width()).
                                                                    arg(rect.height() * 1.0f / group->textureImageSize().height()));

    // copy data onto atlas image
    QPainter painter(&group->image());
    painter.drawImage(rect.x(), rect.y(), node->m_entry->image());
  }

  // clean up
  delete nodeRoot;

  // add group to root element
  root.appendChild(groupElement);

  // save atlas image
  qDebug() << "Generating atlas texture...";
  if ( ! saveImage(group->image(), group->root() + "/" + group->textureImageName(), group->textureFormatName(), group->isAlphaPremultiplied()))
  {
    // error!
    qDebug() << "ERROR: Could not save atlas image!";
    return false;
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Processes Atlas-Group XML Tag. */
bool TextureAtlasGenerator::processAtlasGroupTag(QXmlStreamReader& input)
{
  QString nameAttribute             = input.attributes().value("name").toString();
  QString rootAttribute             = input.attributes().value("root").toString();
  QString textureSizeAttribute      = input.attributes().value("texture-size").toString();
  QString textureFiltersAttribute   = input.attributes().value("texture-filters").toString();
  QString textureImageAttribute     = input.attributes().value("texture-image").toString();
  QString textureFormatAttribute    = input.attributes().value("texture-format").toString();
  QString alphaPremultiplyAttribute = input.attributes().value("alpha-premultiply").toString();

  // set defaults if needed
  if (textureFiltersAttribute.isEmpty())
  {
    textureFiltersAttribute = "bilinear";
  }
  
  if (alphaPremultiplyAttribute.isEmpty())
  {
    alphaPremultiplyAttribute = "false";
  }

  // decompose required attributes
  bool ok;
  QSize size(textureSizeAttribute.section(' ', 0, 0).toInt(&ok), textureSizeAttribute.section(' ', 1, 1).toInt(&ok));

  // check if required data NOT present
  if (nameAttribute.isEmpty() || rootAttribute.isEmpty() || textureSizeAttribute.isEmpty() || textureImageAttribute.isEmpty() || !size.isValid() || !ok)
  {
    // error!
    qDebug() << "WARNING: Skipping atlas group due to lack of required data!";
    return false;
  }

  // create atlas group
  AtlasGroup* group = new AtlasGroup(nameAttribute, rootAttribute, textureImageAttribute, textureFormatAttribute, textureFiltersAttribute, size,
                                     0 == alphaPremultiplyAttribute.compare("true"));
  if (!group || !group->isValid())
  {
    // error!
    qDebug() << "ERROR: Could not create atlas group: " << nameAttribute;

    delete group;
    return false;
  }
    
  // add into pool
  m_groups.push_back(group);

  // process children
  bool done = false;
  while ( ! input.atEnd() && !done)
  {
    QXmlStreamReader::TokenType token = input.readNext();
    switch (token)
    {
      case QXmlStreamReader::StartElement:

        if ("image" == input.name())
        {
          if ( ! processImageTag(input, group))
          {
            // error!
            return false;
          }
        }
        break;

      case QXmlStreamReader::EndElement:

        if ("atlas-group" == input.name())
        {
          // done
          done = true;
        }
        break;
    }
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Processes Image XML Tag. */
bool TextureAtlasGenerator::processImageTag(QXmlStreamReader& input, AtlasGroup* group)
{
  QString nameAttribute     = input.attributes().value("name").toString();
  QString pathAttribute     = input.attributes().value("path").toString();
  QString spacingAttribute  = input.attributes().value("spacing").toString();

  // decompose required attributes
  bool ok;
  QVector4D spacing(spacingAttribute.section(' ', 0, 0).toInt(&ok), spacingAttribute.section(' ', 1, 1).toInt(&ok),
                    spacingAttribute.section(' ', 2, 2).toInt(&ok), spacingAttribute.section(' ', 3, 3).toInt(&ok));

  // create new group entry for an image
  AtlasGroupEntry* entry = new AtlasGroupEntry(nameAttribute, pathAttribute, spacing);
  if ((NULL == entry) || !entry->isValid())
  {
    // error!
    qDebug() << "ERROR: Invalid image! Name:" << nameAttribute << "Path:" << pathAttribute << "Group:" << group->name();
    return false;
  }

  // add to group
  group->addEntry(entry);

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called to begin processing. */
void TextureAtlasGenerator::onStart()
{
  // show header
  printHeader();

  // check if required data is missing
  if (m_inputDataFilePath.isEmpty() || m_outputXmlPath.isEmpty())
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
/*! Saves given image with given format. */
bool TextureAtlasGenerator::saveImage(const QImage& image, const QString& path, const QString& formatName, bool alphaPremultiply) const
{
  //PVRTexTool –f<format> -i<inputfilename> [-a<alphafilename>] [-b<factor>] [-border] [-d] [-
  //dds*] [-dither] [-e] [-h] [-help] [-ktx] [-l] [-m] [-nt] [-ngt] [-o<outputfilename>]
  //[-ob<channelorder>][-p] [-premultalpha] [-pvrlegacy] [-pvrtcmethod] [-pvrtcfast] [-
  //pvrtcnormal] [-pvrtchigh] [-pvrtcbest] [-q<level>] [-r<algorithm>] [-silent] [-square] [-
  //s<filename>] [-x<width>] [-y<height>] [-yflip<0,1>]

  // get requested texture format
  TextureFormat textureFormat = MapImageFormat(formatName);
  QString extensionName       = GetTextureFileExtension(formatName);

  // PVRTC uncompressed formats
  if ((TF_PVRTC_RGBA5551 == textureFormat) || (TF_PVRTC_RGBA4444 == textureFormat))
  {
    // initially save as PNG (RGBA8888)
    if ( ! image.save(path + ".png"))
    {
      // error!
      return false;
    }

    QString program = "PVRTexTool.exe";
    QStringList arguments;
    arguments << QString("-i%1").arg(path + ".png") << QString("-o%1").arg(path + extensionName) << "-yflip0";

    if (TF_PVRTC_RGBA5551 == textureFormat)
    {
      arguments << "-fOGL5551";
    }
    else
    {
      arguments << "-fOGL4444";
    }

    if (alphaPremultiply)
    {
      arguments << "-premultalpha";
    }

    QProcess* process = new QProcess();
    process->start(program, arguments);
    process->waitForFinished(-1);
    delete process;

    // remove original PNG file
    QFile::remove(path + ".png");
  }
  // PVRTC compressed formats
  else if ((TF_PVRTC_2BPP == textureFormat) || (TF_PVRTC_4BPP == textureFormat))
  {
    // initially save as PNG (RGBA8888)
    if ( ! image.save(path + ".png"))
    {
      // error!
      return false;
    }

    QString program = "PVRTexTool.exe";
    QStringList arguments;
    arguments << QString("-i%1").arg(path + ".png") << QString("-o%1").arg(path + extensionName) << "-pvrtcbest" << "-yflip0";

    if (TF_PVRTC_2BPP == textureFormat)
    {
      arguments << "-fOGLPVRTC2";
    }
    else
    {
      arguments << "-fOGLPVRTC4";
    }

    if (alphaPremultiply)
    {
      arguments << "-premultalpha";
    }

    QProcess* process = new QProcess();
    process->start(program, arguments);
    process->waitForFinished(-1);
    delete process;

    // remove original PNG file
    QFile::remove(path + ".png");
  }
  // check if uncompessed RGBA8888
  else if (TF_RGBA8888 == textureFormat)
  {
    // check if alpha should be premultiplied
    if (alphaPremultiply)
    {
      QImage newImage = image.convertToFormat(QImage::Format_ARGB32_Premultiplied);
      if (newImage.isNull())
      {
        // error!
        return false;
      }

      // save it
      if ( ! newImage.save(path + ".png"))
      {
        // error!
        return false;
      }
    }
    else
    {
      // save it
      if ( ! image.save(path + ".png"))
      {
        // error!
        return false;
      }
    }
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

