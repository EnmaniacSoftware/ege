#include "Core/Graphics/Color/ColorTransform.h"
#include "EGEMath.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ColorTransform::ColorTransform() : m_additionComponent(Color::NONE)
                                 , m_multiplicationComponent(Color::WHITE)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ColorTransform::ColorTransform(const Color& addition, const Color& multiplication) : m_additionComponent(addition)
                                                                                   , m_multiplicationComponent(multiplication)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ColorTransform::~ColorTransform()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Color ColorTransform::transform(const Color& color) const
{
  Color result;

  result.red    = Math::Max(0.0f, Math::Min(color.red * m_multiplicationComponent.red + m_additionComponent.red, 1.0f));
  result.green  = Math::Max(0.0f, Math::Min(color.green * m_multiplicationComponent.green + m_additionComponent.green, 1.0f));
  result.blue   = Math::Max(0.0f, Math::Min(color.blue * m_multiplicationComponent.blue + m_additionComponent.blue, 1.0f));
  result.alpha  = Math::Max(0.0f, Math::Min(color.alpha * m_multiplicationComponent.alpha + m_additionComponent.alpha, 1.0f));

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ColorTransform::operator == (const ColorTransform& other) const
{
  return (m_additionComponent == other.m_additionComponent) && (m_multiplicationComponent == other.m_multiplicationComponent);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ColorTransform::operator != (const ColorTransform& other) const
{
  return ! (*this == other);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END