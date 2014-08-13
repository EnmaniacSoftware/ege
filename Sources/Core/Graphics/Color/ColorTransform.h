#ifndef EGE_CORE_GRAPHICS_COLOR_COLORTRANSFORM_H
#define EGE_CORE_GRAPHICS_COLOR_COLORTRANSFORM_H

#include "EGETypes.h"
#include "EGEColor.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ColorTransform
{
  public:

    ColorTransform();
    ColorTransform(const Color& addition, const Color& multiplication);
   ~ColorTransform();

  operators:

    bool operator == (const ColorTransform& other) const;
    bool operator != (const ColorTransform& other) const;

  public:

    /*! Sets addition color transformation. 
     *  @param color Color to add to base color. 
     */
    void setAddition(const Color& color);
    /*! Returns addition color component. */
    const Color& addition() const;

    /*! Sets multiplication color transformation. 
     *  @param color Color to mulitply base color with. 
     */
    void setMultiplication(const Color& color);
    /*! Returns multiplication color component. */
    const Color& multiplication() const;

    /*! Transforms given color. 
     *  @param color  Color to transform.
     *  @return Transformed color.
     *  @note Returned color's components are in range [0-1].
     */
    Color transform(const Color& color) const;

  private:

    /*! Addition component. */
    Color m_additionComponent;
    /*! Multiplication component. */
    Color m_multiplicationComponent;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_GRAPHICS_COLOR_COLORTRANSFORM_H