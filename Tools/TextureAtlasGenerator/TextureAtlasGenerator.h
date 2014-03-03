#ifndef TEXTURE_ATLAS_GENERATOR_H
#define TEXTURE_ATLAS_GENERATOR_H

#include <QApplication>
#include <QString>
#include <QList>
#include <QImage>
#include <QXmlStreamReader>
#include <QDomDocument>
#include "AtlasNode.h"
#include "AtlasGroup.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class TextureAtlasGenerator : public QApplication
{
  Q_OBJECT

	public:

		TextureAtlasGenerator(int argc, char* argv[]);
	 ~TextureAtlasGenerator();

  public:

    /*! Available texture formats. */
    enum TextureFormat
    {
      TF_RGBA8888,        // png
      TF_RGB888,          // png
      TF_PVRTC_RGBA5551,  // uncompressed
      TF_PVRTC_RGBA4444,  // uncompressed
      TF_PVRTC_2BPP,      // 2 bits per pixel PVRTC RGB(A) compression. NOTE, from compressor point of view RGB or RGBA does not matter.
      TF_PVRTC_4BPP       // 4 bits per pixel PVRTC RGB(A) compression. NOTE, from compressor point of view RGB or RGBA does not matter.
    };

  private:

    /*! Available sorting methods. */ 
    enum SortMethod
    {
      SM_NONE,
      SM_GREATER_AREA,
      SM_GREATER_WIDTH,
      SM_GREATER_HEIGHT
    };

  private:

    /* Processes all data. */
    bool process();
    /*! Returns output XML file path. */
    inline const QString& outputXmlPath() const { return m_outputXmlPath; }
    /* Generates atlases. */
    bool generateAll();
    /* Generates atlas for given group. */
    bool generate(QDomDocument& doc, QDomElement& root, AtlasGroup* group);
    /*! Returns current sorting method being used. */
    inline SortMethod sortMethod() const { return m_sortMethod; }
    /* Processes Atlas-Group XML Tag. */
    bool processAtlasGroupTag(QXmlStreamReader& input);
    /* Processes Image XML Tag. */
    bool processImageTag(QXmlStreamReader& input, AtlasGroup* group);
    /* Prints syntax to standard output. */
    void printSyntax() const;
    /* Prints header to standard output. */
    void printHeader() const;
    /* Saves given image with given format. */
    bool saveImage(const QImage& image, const QString& path, const QString& formatName, bool alphaPremultiply) const;
    
  private slots:

    /* Slot called to begin processing. */
    void onStart();

	private:

    /*! Input data file path. */
    QString m_inputDataFilePath;
    /*! Output XML data file path. */
    QString m_outputXmlPath;
    /*! Current sorting method. */
    SortMethod m_sortMethod;
    /*! Atlas groups. */
    QList<AtlasGroup*> m_groups;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // TEXTURE_ATLAS_GENERATOR_H
