#ifndef SWFTYPES_H
#define SWFTYPES_H

#include <QList>
#include <QMap>
#include <QVariant>
#include <QColor>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#define T2P(twips) ((twips) / 20.0f)
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

  static Matrix Identity()
  {
    Matrix t;
    t.scaleX = 1;
    t.scaleY = 1;
    t.rotX = 0;
    t.rotY = 0;
    t.translateX = 0;
    t.translateY = 0;

    return t;
  }

  static Matrix Concatenate(const Matrix& m1, const Matrix& m2)
  {
    Matrix t;

    float	matrix1[2][3];
    float	matrix2[2][3];
    float	out[2][3];

    matrix1[0][0] = m1.scaleX;
    matrix1[1][1] = m1.scaleY;
    matrix1[1][0] = m1.rotX;
    matrix1[0][1] = m1.rotY;
    matrix1[0][2] = m1.translateX;
    matrix1[1][2] = m1.translateY;

    matrix2[0][0] = m2.scaleX;
    matrix2[1][1] = m2.scaleY;
    matrix2[1][0] = m2.rotX;
    matrix2[0][1] = m2.rotY;
    matrix2[0][2] = m2.translateX;
    matrix2[1][2] = m2.translateY;

    out[0][0] = matrix1[0][0] * matrix2[0][0] + matrix1[0][1] * matrix2[1][0];
    out[1][0] = matrix1[1][0] * matrix2[0][0] + matrix1[1][1] * matrix2[1][0];
    out[0][1] = matrix1[0][0] * matrix2[0][1] + matrix1[0][1] * matrix2[1][1];
    out[1][1] = matrix1[1][0] * matrix2[0][1] + matrix1[1][1] * matrix2[1][1];
    out[0][2] = matrix1[0][0] * matrix2[0][2] + matrix1[0][1] * matrix2[1][2] + matrix1[0][2];
    out[1][2] = matrix1[1][0] * matrix2[0][2] + matrix1[1][1] * matrix2[1][2] + matrix1[1][2];

    t.scaleX      = out[0][0];
    t.scaleY      = out[1][1];
    t.rotX        = out[1][0];
    t.rotY        = out[0][1];
    t.translateX  = out[0][2];
    t.translateY  = out[1][2];

    return t;
  }
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

  static ColorTransform Identity()
  {
    ColorTransform t;

    t.addTerms.setRed(0);
    t.addTerms.setGreen(0);
    t.addTerms.setBlue(0);
    t.addTerms.setAlpha(0);

    t.multTerms.setRed(255);
    t.multTerms.setGreen(255);
    t.multTerms.setBlue(255);
    t.multTerms.setAlpha(255);

    return t;
  }

  static ColorTransform Concatenate(const ColorTransform& ct1, const ColorTransform& ct2)
  {
    ColorTransform out = Identity();

		out.addTerms.setRedF(ct1.multTerms.redF() * ct2.addTerms.redF());
		out.addTerms.setGreenF(ct1.multTerms.greenF() * ct2.addTerms.greenF());
		out.addTerms.setBlueF(ct1.multTerms.blueF() * ct2.addTerms.blueF());
		out.addTerms.setAlphaF(ct1.multTerms.alphaF() * ct2.addTerms.alphaF());

    out.multTerms.setRedF(ct1.multTerms.redF() * ct2.multTerms.redF());
    out.multTerms.setGreenF(ct1.multTerms.greenF() * ct2.multTerms.greenF());
    out.multTerms.setBlueF(ct1.multTerms.blueF() * ct2.multTerms.blueF());
    out.multTerms.setAlphaF(ct1.multTerms.alphaF() * ct2.multTerms.alphaF());

    return out;
  }

  QColor transform(const QColor& color) const
  {
    QColor out;
    
    out.setRed(qMax(0, qMin(static_cast<int>((color.red() * multTerms.redF()) + addTerms.red()), 255)));
    out.setGreen(qMax(0, qMin(static_cast<int>((color.green() * multTerms.greenF()) + addTerms.green()), 255)));
    out.setBlue(qMax(0, qMin(static_cast<int>((color.blue() * multTerms.blueF()) + addTerms.blue()), 255)));
    out.setAlpha(qMax(0, qMin(static_cast<int>((color.alpha() * multTerms.alphaF()) + addTerms.alpha()), 255)));

    return out;
  }
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // SWFTYPES_H
