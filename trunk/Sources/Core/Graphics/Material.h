#ifndef EGE_CORE_MATERIAL_H
#define EGE_CORE_MATERIAL_H

#include <EGE.h>
#include <EGEString.h>
#include <EGEDynamicArray.h>
#include "Core/Graphics/Render/RenderPass.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(Material, PMaterial)
EGE_DECLARE_SMART_CLASS(Object, PObject)
EGE_DECLARE_SMART_CLASS(RenderPass, PRenderPass)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Material : public Object
{
  public:

    Material(Application* app);
   ~Material();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Returns TRUE if material is valid. */
    bool isValid() const;

    /*! Returns number of passes. */
    inline u32 passCount() const { return static_cast<u32>(m_passes.size()); }
    /* Returns pass at given index. */
    PRenderPass pass(u32 index) const;
    /*  Adds render pass to material. 
     *  @param pass RenderPass obejct to be added.
     *  @note If NULL object passed in, new render pass object will be created and added.
     *  @return Return added object. NULL if error occured.
     */
    PRenderPass addPass(const PRenderPass& pass);

    /* Sets source pixel blend factor for all passes. */
    void setSrcBlendFactor(EGEGraphics::BlendFactor factor);
    /* Sets destination pixel blend factor for all passes. */
    void setDstBlendFactor(EGEGraphics::BlendFactor factor);

    /* Sets diffuse color for all passes. */
    void setDiffuseColor(const Color& color);
    /* Sets diffuse alpha only for all passes. */
    void setDiffuseAlpha(float32 alpha);
    /* Sets ambient color for all passes. */
    void setAmbientColor(const Color& color);
    /* Sets specular color for all passes. */
    void setSpecularColor(const Color& color);
    /* Sets shininess for all passes. */
    void setShininess(float32 shininess);
    /* Sets emission color for all passes. */
    void setEmissionColor(const Color& color);

    /* Clears material. */
    void clear();

    /* Returns cloned instance of this object. */
    PMaterial clone() const;

  private:

    typedef DynamicArray<PRenderPass> PassArray;

  private:

    /*! Render passes. */
    PassArray m_passes;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_MATERIAL_H
