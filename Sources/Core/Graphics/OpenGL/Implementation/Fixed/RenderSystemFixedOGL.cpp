#include "Core/Graphics/OpenGL/Implementation/Fixed/RenderSystemFixedOGL.h"
#include "Core/Graphics/OpenGL/Implementation/VertexArrayObject.h"
#include "Core/Graphics/OpenGL/Texture2DOGL.h"
#include "Core/Graphics/Render/Implementation/RenderSystemStatistics.h"
#include "EGEDevice.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Maps texture environment to OpenGL compilant one. */
static GLint MapEnvironmentMode(TextureEnvironmentMode mode)
{
  GLint result = GL_MODULATE;

  switch (mode)
  {
    case EM_ADD:      result = GL_ADD; break;
    case EM_BLEND:    result = GL_BLEND; break;
    //case EGETexture::EM_COMBINE:  result = GL_COMBINE; break;
    case EM_DECAL:    result = GL_DECAL; break;
    case EM_MODULATE: result = GL_MODULATE; break;
    case EM_REPLACE:  result = GL_REPLACE; break;

    default:
      break;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderSystemFixedOGL::RenderSystemFixedOGL(Engine& engine) : RenderSystemOGL(engine)
                                                           , m_activeClientTextureUnit(0)
{
  glGetIntegerv(GL_MATRIX_MODE, &m_matrixMode);
  OGL_CHECK()
  
  TextureUnitState defaultUnitState;
  defaultUnitState.m_textureCoordIndex = 0;
  for (u32 i = 0; i < Device::TextureUnitsCount(); ++i)
  {
    m_textureUnitStates.push_back(defaultUnitState);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
RenderSystemFixedOGL::~RenderSystemFixedOGL()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystemFixedOGL::flush()
{
  PRenderTarget renderTarget = currentRenderTarget();
  EGE_ASSERT(NULL != renderTarget);

  // setup projection matrix
  setMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // check if not auto-rotated
  if ( ! renderTarget->isAutoRotated())
  {
    // apply rotation
    // NOTE: actual OGLES rotation should be opposite
    glRotatef(-renderTarget->orientationRotation().degrees(), 0, 0, 1);
  }
  
  // apply projection matrix
  glMultMatrixf(m_projectionMatrix.data);

  // call base class
  RenderSystemOGL::flush();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystemFixedOGL::applyPassParams(const PRenderPass& pass)
{
  const PRenderComponent& component = activeRenderComponent();

  EGE_ASSERT(NULL != pass);
  EGE_ASSERT(NULL != component);

  // enable blending if necessary
  if ((EGEGraphics::BF_ONE != pass->srcBlendFactor()) || (EGEGraphics::BF_ZERO != pass->dstBlendFactor()))
  {
    setBlendEnabled(true);
      
    glBlendFunc(mapBlendFactor(pass->srcBlendFactor()), mapBlendFactor(pass->dstBlendFactor()));
    OGL_CHECK()
  }
  else
  {
    setBlendEnabled(false);
  }

  // set vertex color
  // NOTE: this will be overriden if color array is activated
  Color color = pass->diffuseColorTransformation().transform(pass->diffuseColor());
  glColor4f(color.red, color.green, color.blue, color.alpha);

  // go thru all textures
  for (u32 i = 0; i < pass->textureCount(); ++i)
  {
    PTextureImage textureImage = pass->texture(i);
    EGE_ASSERT(EGE_OBJECT_UID_TEXTURE_IMAGE == textureImage->uid());

    // set texture coord array index to be used by current texture unit
    m_textureUnitStates[i].m_textureCoordIndex = textureImage->textureCoordIndex();

    // check if texture image
    if (EGE_OBJECT_UID_TEXTURE_IMAGE == textureImage->uid())
    {
      Texture2DOGL* tex2d = ege_cast<Texture2DOGL*>(textureImage->texture());

      activateTextureUnit(i);

      // NOTE: it is possible texure object might be not present ie when it is manual and hasnt been set yet
      if (NULL != tex2d)
      {
        bindTexture(GL_TEXTURE_2D, tex2d->id());
      }

      glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, MapEnvironmentMode(textureImage->environmentMode()));
      OGL_CHECK()

      // compose texture matrix
      setMatrixMode(GL_TEXTURE);
      glLoadIdentity();

      // apply rotation
      float32 degrees = textureImage->rotationAngle().degrees();
      if (0.0f != degrees)
      {
        glTranslatef(textureImage->rect().x + textureImage->rect().width * 0.5f, textureImage->rect().y + textureImage->rect().height * 0.5f, 0.0f);
        glRotatef(degrees, 0.0f, 0.0f, 1.0f);
        glTranslatef(-(textureImage->rect().x + textureImage->rect().width * 0.5f), -(textureImage->rect().y + textureImage->rect().height * 0.5f), 0.0f);
      }

      // apply translation and scale
      glTranslatef(textureImage->rect().x, textureImage->rect().y, 0.0f);
      glScalef(textureImage->rect().width, textureImage->rect().height, 1.0f);
    }

    // check if points are be rendered
    if (EGEGraphics::RPT_POINTS == component->primitiveType())
    {
      // check if point sprites are supported
      if (Device::HasRenderCapability(ERenderCapabilityPointSprites))
      {
        glEnable(GL_POINT_SPRITE);
        glTexEnvf(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
        OGL_CHECK()
      }
    }
  }
  
  // disable rest texture units
  m_activeTextureUnitsCount = pass->textureCount();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystemFixedOGL::activateClientTextureUnit(u32 unit)
{
  if (unit != m_activeClientTextureUnit)
  {
    if (glClientActiveTexture)
    {
      glClientActiveTexture(GL_TEXTURE0 + unit);
      OGL_CHECK()
    }
    
    m_activeClientTextureUnit = unit;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystemFixedOGL::applyGeneralParams()
{
  const PRenderComponent& component = activeRenderComponent();

  // call base class
  RenderSystemOGL::applyGeneralParams();

  // check if points are to be rendered
  if (EGEGraphics::RPT_POINTS == component->primitiveType())
  {
    // set point size
    glPointSize(component->pointSize());
  }
  // check if lines are to be rendered
  else if ((EGEGraphics::RPT_LINES == component->primitiveType()) || (EGEGraphics::RPT_LINES == component->primitiveType()))
  {
    glLineWidth(component->lineWidth());
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PShader RenderSystemFixedOGL::createShader(EGEGraphics::ShaderType type, const String& name, const PDataBuffer& data)
{
  EGE_UNUSED(type);
  EGE_UNUSED(name);
  EGE_UNUSED(data);

  // not available
  return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PProgram RenderSystemFixedOGL::createProgram(const String& name, const List<PShader>& shaders)
{
  EGE_UNUSED(name);
  EGE_UNUSED(shaders);

  // not available
  return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystemFixedOGL::renderComponent(const PRenderComponent& component, const Matrix4f& modelMatrix)
{
  RenderSystemFrameStatisticData& statisticsData = ege_cast<RenderSystemStatistics*>(this->component(EGE_OBJECT_UID_RENDER_SYSTEM_STATISTICS))->currentRecord();

  // set component being rendered
  setActiveRenderComponent(component);

  PVertexBuffer vertexBuffer = component->vertexBuffer();
  PIndexBuffer indexBuffer   = component->indexBuffer();
  PMaterial material         = component->material();

  EGE_ASSERT_X(NULL != material, "Component must have material attached.");
  EGE_ASSERT_X(0 < vertexBuffer->vertexCount(), "No vertices to render!");

  // make sure VAOs are created and set up
  createAndSetupVAOs();

  // get list of vaos
  // NOTE: each VAO in the list corresponds to single pass
  List<PComponent> vaos = component->components(EGE_OBJECT_UID_VERTEX_ARRAY_OBJECT);

  // check if no VAOs present
  if (vaos.isEmpty())
  {
    // bind vertex and index buffers
    // NOTE: this is valid for all passes
    vertexBuffer->bind();
    indexBuffer->bind();
  }

  // apply general params
  applyGeneralParams();

  // go thru all passes
  for (u32 pass = 0; pass < material->passCount(); ++pass)
  {
    const PRenderPass renderPass = material->pass(pass);

    // apply pass related params
    applyPassParams(renderPass);

    // TAGE - try to move to applyPassParams

    // get first vao from the list and shrink the list
    PVertexArrayObject vao = vaos.first(NULL);
    if ( ! vaos.isEmpty())
    {
      vaos.removeFirst();
    }

    // apply vertex arrays
    if (NULL == vao)
    {
      applyVertexArrays();
    }
    else
    {
      // bind vao
      vao->bind();
    }

    // NOTE: change to modelview after material is applied as it may change current matrix mode
    setMatrixMode(GL_MODELVIEW);

    // determine number of vertices to render
    const u32 vertexCount = (0 < indexBuffer->indexCount()) ? indexBuffer->indexCount() : vertexBuffer->vertexCount();

    // update statistics
    statisticsData.vertexCount += vertexCount;
    statisticsData.batchCount++;
    statisticsData.queues.rbegin()->indexedBatchCount += (0 < indexBuffer->indexCount()) ? 1 : 0;
    statisticsData.queues.rbegin()->batchCount++;
    statisticsData.queues.rbegin()->vertexCount += vertexCount;
    statisticsData.queues.rbegin()->componentNames << component->name();

    // set model-view matrix
    glLoadMatrixf(m_viewMatrix.multiply(modelMatrix).data);
    OGL_CHECK()

    // check if INDICIES are to be used
    if (0 < indexBuffer->indexCount())
    {
      // render only if there is anything to render
      glDrawElements(mapPrimitiveType(component->primitiveType()), indexBuffer->indexCount(), mapIndexSize(indexBuffer->size()), indexBuffer->offset());
      OGL_CHECK()

      // update statistics
      statisticsData.drawElementsCalls++;
    }
    else
    {
      // render only if there is anything to render
      glDrawArrays(mapPrimitiveType(component->primitiveType()), 0, vertexBuffer->vertexCount());
      OGL_CHECK()

      // update statistics
      statisticsData.drawArraysCalls++;
    }

    if (NULL != vao)
    {
      // unbind vao
      vao->unbind();
    }

    // clean up
    for (s32 i = static_cast<s32>(m_textureUnitStates.size()) - 1; i >= 0; --i)
    {
      // disable texturing on server side
      activateTextureUnit(i);
      glDisable(GL_TEXTURE_2D);
      OGL_CHECK()

      // disable texturing data on client side
      if (NULL == vao)
      {
        activateClientTextureUnit(i);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        OGL_CHECK()
      }

      // disable point sprites
      if (EGEGraphics::RPT_POINTS == component->primitiveType())
      {
        if (Device::HasRenderCapability(ERenderCapabilityPointSprites))
        {
          glDisable(GL_POINT_SPRITE);
          OGL_CHECK()
        }
      }
    }
  }

  if (vaos.isEmpty())
  {
    // unbind vertex and index buffers
    vertexBuffer->unbind();
    indexBuffer->unbind();
  }

  // reset component being rendered
  setActiveRenderComponent(NULL);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystemFixedOGL::setClientStateEnabled(u32 state, bool set)
{
  bool isSet = m_activeClientStates.contains(state);

  if (set && ! isSet)
  {
    glEnableClientState(static_cast<GLenum>(state));
    OGL_CHECK()

    m_activeClientStates.push_back(state);
  }
  else if ( ! set && isSet)
  {
    glDisableClientState(static_cast<GLenum>(state));
    OGL_CHECK()

    m_activeClientStates.remove(state);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystemFixedOGL::applyVertexArrays()
{
  const PRenderComponent& component = activeRenderComponent();

  EGE_ASSERT(NULL != component);
  EGE_ASSERT(NULL != component->vertexBuffer());

  const PVertexBuffer& vertexBuffer = component->vertexBuffer();

  bool vertexPointerInUse = false;
  bool normalPointerInUse = false;
  bool colorPointerInUse  = false;

  const VertexDeclaration& vertexDeclaration = vertexBuffer->vertexDeclaration();
  s8* vertexData = reinterpret_cast<s8*>(vertexBuffer->offset());

  // go thru all arrays
  const VertexElementArray& vertexElements = vertexDeclaration.vertexElements();
  for (VertexElementArray::const_iterator itElement = vertexElements.begin(); itElement != vertexElements.end(); ++itElement)
  {
    // set according to buffer type
    switch (itElement->semantic())
    {
      case NVertexBuffer::VES_POSITION_XYZ:

        glVertexPointer(3, GL_FLOAT, vertexDeclaration.vertexSize(), vertexData + itElement->offset());
        OGL_CHECK()
        vertexPointerInUse = true;
        break;

      case NVertexBuffer::VES_POSITION_XY:

        glVertexPointer(2, GL_FLOAT, vertexDeclaration.vertexSize(), vertexData + itElement->offset());
        OGL_CHECK()
        vertexPointerInUse = true;
        break;

      case NVertexBuffer::VES_NORMAL:

        glNormalPointer(GL_FLOAT, vertexDeclaration.vertexSize(), vertexData + itElement->offset());
        OGL_CHECK()
        normalPointerInUse = true;
        break;

      case NVertexBuffer::VES_COLOR_RGBA:

        glColorPointer(4, GL_FLOAT, vertexDeclaration.vertexSize(), vertexData + itElement->offset());
        OGL_CHECK()
        colorPointerInUse = true;
        break;

      case NVertexBuffer::VES_TEXTURE_UV:

        for (u32 i = 0; i < m_activeTextureUnitsCount; ++i)
        {
          // set only texture units which are to use current texture coords array
          if (m_textureUnitStates[i].m_textureCoordIndex == itElement->index())
          {
            activateClientTextureUnit(i);

            glTexCoordPointer(2, GL_FLOAT, vertexDeclaration.vertexSize(), vertexData + itElement->offset());
            OGL_CHECK()

            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            OGL_CHECK()
          }
        }
        break;

      default:
        break;
    }
  }

  // enable/disable vertex pointers
  setClientStateEnabled(GL_VERTEX_ARRAY, vertexPointerInUse);
  setClientStateEnabled(GL_NORMAL_ARRAY, normalPointerInUse);
  setClientStateEnabled(GL_COLOR_ARRAY, colorPointerInUse);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystemFixedOGL::setMatrixMode(s32 mode)
{
  if (mode != m_matrixMode)
  {
    glMatrixMode(mode);
    OGL_CHECK()

    m_matrixMode = mode;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void RenderSystemFixedOGL::setupVAO(PVertexArrayObject& vertexArrayObject, const PVertexBuffer& vertexBuffer, const PIndexBuffer& indexBuffer,
                                    const PRenderPass& pass)
{
  EGE_UNUSED(pass);

  // bind VAO
  // NOTE: Currently, all vertex states are stored within VAO rather than in OpenGL state machine, so there is no need to unbind anything 
  vertexArrayObject->bind();

  // bind VBO
  vertexBuffer->bind();

  // go thru all arrays
  s32 textureCoordIndex = 0;
  const VertexElementArray& vertexElements = vertexBuffer->vertexDeclaration().vertexElements();
  for (VertexElementArray::const_iterator itElement = vertexElements.begin(); itElement != vertexElements.end(); ++itElement)
  {
    // set according to buffer type
    switch (itElement->semantic())
    {
      case NVertexBuffer::VES_POSITION_XYZ:

        glVertexPointer(3, GL_FLOAT, vertexBuffer->vertexDeclaration().vertexSize(), static_cast<s8*>(vertexBuffer->offset()) + itElement->offset());
        OGL_CHECK()
        glEnableClientState(GL_VERTEX_ARRAY);
        OGL_CHECK()
        break;

      case NVertexBuffer::VES_POSITION_XY:

        glVertexPointer(2, GL_FLOAT, vertexBuffer->vertexDeclaration().vertexSize(), static_cast<s8*>(vertexBuffer->offset()) + itElement->offset());
        OGL_CHECK()
        glEnableClientState(GL_VERTEX_ARRAY);
        OGL_CHECK()
        break;

      case NVertexBuffer::VES_NORMAL:

        glNormalPointer(GL_FLOAT, vertexBuffer->vertexDeclaration().vertexSize(), static_cast<s8*>(vertexBuffer->offset()) + itElement->offset());
        OGL_CHECK()
        glEnableClientState(GL_NORMAL_ARRAY);
        OGL_CHECK()
        break;

      case NVertexBuffer::VES_COLOR_RGBA:

        glColorPointer(4, GL_FLOAT, vertexBuffer->vertexDeclaration().vertexSize(), static_cast<s8*>(vertexBuffer->offset()) + itElement->offset());
        OGL_CHECK()
        glEnableClientState(GL_COLOR_ARRAY);
        OGL_CHECK()
        break;

      case NVertexBuffer::VES_TEXTURE_UV:

        activateClientTextureUnit(textureCoordIndex);

        glTexCoordPointer(2, GL_FLOAT, vertexBuffer->vertexDeclaration().vertexSize(), static_cast<s8*>(vertexBuffer->offset()) + itElement->offset());
        OGL_CHECK()
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        OGL_CHECK()
        break;

      default:
        break;
    }
  }

  // bind IBO
  indexBuffer->bind();

  // unbind VAO
  vertexArrayObject->unbind();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END