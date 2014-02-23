#ifndef EGE_CORE_GRAPHICS_OPENGL_VERTEXARRAYOBJECT_H
#define EGE_CORE_GRAPHICS_OPENGL_VERTEXARRAYOBJECT_H

/** 
 *   VertexArrayObject encapsulates OpenGL set of states previously configured for vertex data rendering.
 */

#include "EGE.h"
#include "EGEComponent.h"
#include "EGEOpenGL.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(VertexArrayObject, PVertexArrayObject)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class VertexArrayObject : public Component
{
  public:

    VertexArrayObject(Application* app, const String& name);
    virtual ~VertexArrayObject();

    /*! Binds object. */
    void bind();
    /*! Unbinds object. */
    void unbind();

  private:

    /*! Object ID. */
    GLuint m_id;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_GRAPHICS_OPENGL_VERTEXARRAYOBJECT_H
