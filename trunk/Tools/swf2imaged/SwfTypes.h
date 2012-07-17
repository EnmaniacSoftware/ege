#ifndef SWFTYPES_H
#define SWFTYPES_H

#include <QList>
#include <QMap>
#include <QVariant>
#include <QColor>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef QMap<quint16, QObject*> Dictionary;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Available fill style types. */
enum FillStyleTypes
{
  FST_SOLID                       = 0x00,
  FST_LINEAR_GRADIENT             = 0x10,
  FST_RADIAL_GRADIENT             = 0x12,
  FST_FOCAL_RADIAL_GRADIENT       = 0x13,
  FST_REPEAT_BITMAP               = 0x40,
  FST_CLIPPED_BITMAP              = 0x41,
  FST_NONSMOOTHED_REPEAT_BITMAP   = 0x42,
  FST_NONSMOOTHED_CLIPPED_BITMAP  = 0x43
};

/*! Available gradient spread modes. */
enum GradientSpreadMode
{
  GSM_PAD = 0,
  GSM_REFELCT = 1,
  GSM_REPEAT = 2
};

/*! Available gradient interpolation modes. */
enum GradientInterpolationMode
{
  GIM_NORMAL_RGB = 0,
  GIM_LINEAR_RGB = 1
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Data struct representing gradient control points. */
struct GradientRecord
{
  quint8 ratio;                     /*!< Ration value. */
  QColor color;                     /*!< Gradient color. RGBA for Shape3, RGB for Shape1 and Shape2. */
};

/*! Data struct representing gradient. */
struct Gradient
{
  quint8 spreadMode;                      /*!< Spread mode (GradientSpreadMode). */
  quint8 interpolationMode;               /*!< Interpolation mode (GradientInterpolationMode). */
  QList<GradientRecord> gradientPoints;   /*!< List of all gradients control points. */
};

/*! Data struct representing focal gradient. */
struct FocalGradient
{
  quint8 spreadMode;                      /*!< Spread mode (GradientSpreadMode). */
  quint8 interpolationMode;               /*!< Interpolation mode (GradientInterpolationMode). */
  QList<GradientRecord> gradientPoints;   /*!< List of all gradients control points. */
  float focalPoint;                       /*!< Focal point location (FP8). */
};

/*! Data struct representing 2D Matrix. */
struct Matrix
{
  float scaleX;
  float scaleY;
  float rotX;
  float rotY;
  qint32 translateX;            /*!< in twips. */
  qint32 translateY;            /*!< in twips. */
};

/*! Data struct representing fill style. */
struct FillStyle
{
  quint8 type;                  /*!< Type of fill (FillStyleTypes). */
  QColor color;                 /*!< Solid color. If type == FST_SOLID -> RGBA (for Shape3), RGB (Shape1 and Shape2). */
  
  Matrix gradientMatrix;        /*!< Matrix for gradient fill (if type == FST_LINEAR_GRADIENT, FST_RADIAL_GRADIENT or FST_FOCAL_RADIAL_GRADIENT). */
  Gradient gradient;            /*!< If type == FST_LINEAR_GRADIENT or FST_RADIAL_GRADIENT. */
  FocalGradient focalGradient;  /*!< If type == FST_FOCAL_RADIAL_GRADIENT. */

  quint16 bitmapCharacterId;    /*!< Id of bitmap character for fill. 
                                     If type == FST_REPEAT_BITMAP, FST_CLIPPED_BITMAP, FST_NONSMOOTHED_REPEAT_BITMAP or FST_NONSMOOTHED_CLIPPED_BITMAP. */
  Matrix bitmapMatrix;          /*!< Matrix for bitmap fill.
                                     If type == FST_REPEAT_BITMAP, FST_CLIPPED_BITMAP, FST_NONSMOOTHED_REPEAT_BITMAP or FST_NONSMOOTHED_CLIPPED_BITMAP. */
  // NOTE: it seems translation needs to be applied to object matrix
};

/*! Data struct representing line style. */
struct LineStyle
{
  quint16 width;            /*!< Width of line (in twips). */
  QColor  color;            /*!< Line color. RGBA for Shape3, RGB for Shape2 and Shape1. */
};

/*! Data struct representing shape with style. */
struct ShapeWithStyle
{
  QList<FillStyle> fillStyles;      /*!< List of fill styles. */
  QList<LineStyle> lineStyles;      /*!< List of line styles. */

  qint8 numFillBits;                /*!< Number of fill index bits. */
  qint8 numLineBits;                /*!< Number of line index bits. */
};

/*! Color tranformation data struct. */
struct ColorTransform
{
  QColor addTerms;
  QColor multTerms;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // SWFTYPES_H
