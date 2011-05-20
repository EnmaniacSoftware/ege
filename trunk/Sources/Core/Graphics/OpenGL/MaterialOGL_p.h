#ifndef EGE_CORE_MATERIAL_PRIVATE_H
#define EGE_CORE_MATERIAL_PRIVATE_H

#include "EGE.h"
#include "EGEString.h"

#ifdef EGE_PLATFORM_WIN32
#include <gl/GL.h>
#else
#include "GLES/gl.h"
#endif

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Material;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class MaterialPrivate
{
  public:

    MaterialPrivate(Material* base);
   ~MaterialPrivate();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Sets source pixel scale function. */
    void setSrcBlendFunc(const EGEString& funcName);
    /*! Gets source pixel scale function. */
    inline GLenum srcBlendFunc() const { return m_srcBlendFunc; }
    /* Sets destination pixel scale function. */
    void setDstBlendFunc(const EGEString& funcName);
    /*! Gets destination pixel scale function. */
    inline GLenum dstBlendFunc() const { return m_dstBlendFunc; }

  private:

    /*! Pointer to base object. */
    Material* m_base;
    /*! Source blend scale function. */
    GLenum m_srcBlendFunc;
    /*! Destination blend scale function. */
    GLenum m_dstBlendFunc;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_MATERIAL_PRIVATE_H
