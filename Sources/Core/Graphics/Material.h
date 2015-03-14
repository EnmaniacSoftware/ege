#ifndef EGE_CORE_GRAPHICS_MATERIAL_H
#define EGE_CORE_GRAPHICS_MATERIAL_H

#include "EGE.h"
#include "EGEString.h"
#include "EGEDynamicArray.h"
#include "EGEColorTransform.h"
#include "Core/Graphics/Render/RenderPass.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(Material, PMaterial)
EGE_DECLARE_SMART_CLASS(RenderPass, PRenderPass)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Material : public Object
{
  public:

    Material();
   ~Material();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  operators:

    bool operator == (const Material& other) const;
    bool operator != (const Material& other) const;

  public:

    /*! Returns TRUE if material is valid. 
     *  @note Material is considered valid if at least one pass is defined.
     */
    bool isValid() const;

    /*! Returns number of passes. */
    u32 passCount() const;
    /*! Returns pass at given index. */
    PRenderPass pass(u32 index) const;
    /*! Adds render pass to material. 
     *  @param pass RenderPass obejct to be added.
     *  @note If NULL object passed in, new render pass object will be created and added.
     *  @return Return added object. NULL if error occured.
     */
    PRenderPass addPass(const PRenderPass& pass);

    /*! Sets source pixel blend factor. 
     *  @param factor     Blend factor to set.
     *  @param passIndex  Pass index for which value is to be set.
     *  @note If negative index is passed value is set for ALL passes.
     */
    void setSrcBlendFactor(EGEGraphics::BlendFactor factor, s32 passIndex = -1);
    /*! Sets destination pixel blend factor. 
     *  @param factor     Blend factor to set.
     *  @param passIndex  Pass index for which value is to be set.
     *  @note If negative index is passed value is set for ALL passes.
     */
    void setDstBlendFactor(EGEGraphics::BlendFactor factor, s32 passIndex = -1);

    /*! Sets diffuse color. 
     *  @param color      Color to set.
     *  @param passIndex  Pass index for which value is to be set.
     *  @note If negative index is passed value is set for ALL passes.
     */
    void setDiffuseColor(const Color& color, s32 passIndex = -1);
    /*! Sets diffuse alpha. 
     *  @param color      Color to set.
     *  @param passIndex  Pass index for which value is to be set.
     *  @note If negative index is passed value is set for ALL passes.
     */
    void setDiffuseAlpha(float32 alpha, s32 passIndex = -1);
    /*! Sets diffuse color transformation.
     *  @param color      Color to set.
     *  @param passIndex  Pass index for which value is to be set.
     *  @note If negative index is passed value is set for ALL passes.
     */
    void setDiffuseColorTransformation(const ColorTransform& transformation, s32 passIndex = -1);
    /*! Sets ambient color. 
     *  @param transformation Value to set.
     *  @param passIndex      Pass index for which value is to be set.
     *  @note If negative index is passed value is set for ALL passes.
     */
    void setAmbientColor(const Color& color, s32 passIndex = -1);
    /*! Sets specular color. 
     *  @param color      Color to set.
     *  @param passIndex  Pass index for which value is to be set.
     *  @note If negative index is passed value is set for ALL passes.
     */
    void setSpecularColor(const Color& color, s32 passIndex = -1);
    /*! Sets shininess factor.
     *  @param shininess  Value to set.
     *  @param passIndex  Pass index for which value is to be set.
     *  @note If negative index is passed value is set for ALL passes.
     */
    void setShininess(float32 shininess, s32 passIndex = -1);
    /*! Sets emission color. 
     *  @param color      Color to set.
     *  @param passIndex  Pass index for which value is to be set.
     *  @note If negative index is passed value is set for ALL passes.
     */
    void setEmissionColor(const Color& color, s32 passIndex = -1);

    /*! Clears material. */
    void clear();

    /*! Returns cloned instance of this object. */
    PMaterial clone() const;

  private:

    typedef DynamicArray<PRenderPass> PassArray;

  private:

    /*! Render passes. */
    PassArray m_passes;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
inline u32 Material::passCount() const 
{ 
  return static_cast<u32>(m_passes.length()); 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_NAMESPACE_END

#endif // EGE_CORE_GRAPHICS_MATERIAL_H
