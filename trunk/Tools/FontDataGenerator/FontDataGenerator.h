#ifndef FONT_GENERATOR_H
#define FONT_GENERATOR_H

#include <QtGui/QApplication>
#include <QString>
#include <QColor>
#include <QRect>
#include <QList>
#include <QBitArray>
#include <QImage>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class FontDataGenerator : public QApplication
{
  Q_OBJECT

  public:
  
    FontDataGenerator(int argc, char *argv[]);
    ~FontDataGenerator();
    
  private:

    /* Starts process. */
    bool process();
    /* Processes the region.
       @param   startX          Left position of the region to process within font image (in pixels).
       @param   startY          Top position of the region to process within font image (in pixels).
       @param   image           Image on which region is to be processed.
       @param   processedPixels Bit array containing bits already processed.
       @note    Method determines the extent of the region based on boarder color. Region is then added into glyphs list.
     */
    void processRegion(int startX, int startY, const QImage& image, QBitArray& processedPixels);
    /* Prints syntax to standard output. */
    void printSyntax() const;
    /* Prints header to standard output. */
    void printHeader() const;

  private slots:

    /* Slot called to begin processing. */
    void onStart();

  private:

    /*! Region data struct. */
    struct RegionData
    {
        QRect rect;            /*!< Distinct rectangular area within the image (in pixels). */
    };

  private:

    /*! Input image file name. */
    QString m_imageFileName;
    /*! Input characters data file name. */
    QString m_charsFileName;
    /*! Output data file name. */
    QString m_outputFileName;
    /*! Font name. */
    QString m_fontName;
    /*! Material name. */
    QString m_materialName;
    /*! Border color used to seperate characters within input image. */
    QColor m_borderColor;
    /*!< List of all regions in the order of appearance. */
    QList<RegionData> m_regionList;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // FONT_GENERATOR_H
