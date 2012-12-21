#ifdef EGE_PHYSICS_BOX2D

#include "Core/Application/Application.h"
#include "Core/Physics/Box2D/DebugDrawBox2D.h"
#include "Core/Physics/Box2D/PhysicsManagerBox2D_p.h"
#include <EGEGraphics.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(DebugDraw)
EGE_DEFINE_DELETE_OPERATORS(DebugDraw)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DebugDraw::DebugDraw(Application* app, PhysicsManagerPrivate* managerPrivate) : b2DebugDraw(), 
                                                                                m_app(app), 
                                                                                m_managerPrivate(managerPrivate)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DebugDraw::~DebugDraw()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
  PRenderComponent component = ege_new RenderComponent(app(), "DebugDraw::DrawPolygon", EGEGraphics::RP_PHYSICS_DEBUG, 
                                                       EGEGraphics::RPT_LINE_LOOP);
  if (component->isValid())
  {
    float32 scale = manager()->simulationToWorldScaleFactor();
 
    component->vertexBuffer()->setSemantics(EGEVertexBuffer::ST_V3_C4);

    float32* data = (float32*) component->vertexBuffer()->lock(0, vertexCount);

	  for (int32 i = 0; i < vertexCount; ++i)
	  {
      *data++ = vertices[i].x * scale;
      *data++ = vertices[i].y * scale;
      *data++ = 1.0f;
      *data++ = color.r;
      *data++ = color.g;
      *data++ = color.b;
      *data++ = 1.0f;
    }

    component->vertexBuffer()->unlock(data - 1);

    app()->graphics()->renderer()->addForRendering(component);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
  PRenderComponent component_fill = ege_new RenderComponent(app(), "DebugDraw::DrawSolidPolygon-fill", EGEGraphics::RP_PHYSICS_DEBUG, 
                                                            EGEGraphics::RPT_TRIANGLE_FAN);
  PRenderComponent component_frame = ege_new RenderComponent(app(), "DebugDraw::DrawSolidPolygon-frame", EGEGraphics::RP_PHYSICS_DEBUG, 
                                                            EGEGraphics::RPT_LINE_LOOP);

  PMaterial material = ege_new Material(NULL);
  RenderPass* pass = material->addPass(NULL);
  pass->setSrcBlendFactor(EGEGraphics::BF_SRC_ALPHA);
  pass->setDstBlendFactor(EGEGraphics::BF_ONE_MINUS_SRC_ALPHA);
  component_fill->setMaterial(material);
  
  if (component_fill->isValid() && component_frame->isValid())
  {
    float32 scale = manager()->simulationToWorldScaleFactor();
 
    component_fill->vertexBuffer()->setSemantics(EGEVertexBuffer::ST_V3_C4);
    component_frame->vertexBuffer()->setSemantics(EGEVertexBuffer::ST_V3_C4);
    
    float32* data_fill  = (float32*) component_fill->vertexBuffer()->lock(0, vertexCount);
    float32* data_frame = (float32*) component_frame->vertexBuffer()->lock(0, vertexCount);

	  for (int32 i = 0; i < vertexCount; ++i)
	  {
      *data_fill++ = vertices[i].x * scale;
      *data_fill++ = vertices[i].y * scale;
      *data_fill++ = 1.0f;
      *data_fill++ = color.r;
      *data_fill++ = color.g;
      *data_fill++ = color.b;
      *data_fill++ = 0.5f;

      *data_frame++ = vertices[i].x * scale;
      *data_frame++ = vertices[i].y * scale;
      *data_frame++ = 1.0f;
      *data_frame++ = color.r;
      *data_frame++ = color.g;
      *data_frame++ = color.b;
      *data_frame++ = 0.5f;
    }

    component_fill->vertexBuffer()->unlock(data_fill - 1);
    component_frame->vertexBuffer()->unlock(data_frame - 1);

    app()->graphics()->renderer()->addForRendering(component_fill);
    app()->graphics()->renderer()->addForRendering(component_frame);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void DebugDraw::DrawCircle(const b2Vec2& center, EGE::float32 radius, const b2Color& color)
{
  PRenderComponent component = ege_new RenderComponent(app(), "DebugDraw::DrawCircle", EGEGraphics::RP_PHYSICS_DEBUG, 
                                                       EGEGraphics::RPT_LINE_LOOP);
  if (component->isValid())
  {
	  const float32 k_segments = 16.0f;
	  const float32 k_increment = 2.0f * b2_pi / k_segments;
	  float32 theta = 0.0f;

    float32 scale = manager()->simulationToWorldScaleFactor();
 
    component->vertexBuffer()->setSemantics(EGEVertexBuffer::ST_V3_C4);

    float32* data = (float32*) component->vertexBuffer()->lock(0, (u32) k_segments);

	  for (int32 i = 0; i < k_segments; ++i)
	  {
  		b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
  
      *data++ = v.x * scale;
      *data++ = v.y * scale;
      *data++ = 1.0f;
      *data++ = color.r;
      *data++ = color.g;
      *data++ = color.b;
      *data++ = 1.0f;

  		theta += k_increment;
    }

    component->vertexBuffer()->unlock(data - 1);

    app()->graphics()->renderer()->addForRendering(component);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void DebugDraw::DrawSolidCircle(const b2Vec2& center, EGE::float32 radius, const b2Vec2& axis, const b2Color& color)
{
  PRenderComponent component_fill = ege_new RenderComponent(app(), "DebugDraw::DrawSolidCircle-fill", EGEGraphics::RP_PHYSICS_DEBUG, 
                                                            EGEGraphics::RPT_TRIANGLE_FAN);
  PRenderComponent component_frame = ege_new RenderComponent(app(), "DebugDraw::DrawSolidCircle-frame", EGEGraphics::RP_PHYSICS_DEBUG, 
                                                            EGEGraphics::RPT_LINE_LOOP);

  PMaterial material = ege_new Material(NULL);
  RenderPass* pass = material->addPass(NULL);
  pass->setSrcBlendFactor(EGEGraphics::BF_SRC_ALPHA);
  pass->setDstBlendFactor(EGEGraphics::BF_ONE_MINUS_SRC_ALPHA);
  component_fill->setMaterial(material);

  if (component_fill->isValid() && component_frame->isValid())
  {
	  const float32 k_segments = 16.0f;
	  const float32 k_increment = 2.0f * b2_pi / k_segments;
	  float32 theta = 0.0f;

    float32 scale = manager()->simulationToWorldScaleFactor();
 
    component_fill->vertexBuffer()->setSemantics(EGEVertexBuffer::ST_V3_C4);
    component_frame->vertexBuffer()->setSemantics(EGEVertexBuffer::ST_V3_C4);
    
    float32* data_fill  = (float32*) component_fill->vertexBuffer()->lock(0, (u32) k_segments);
    float32* data_frame = (float32*) component_frame->vertexBuffer()->lock(0, (u32) k_segments);

	  for (int32 i = 0; i < (int32) k_segments; ++i)
	  {
	  	b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));

      *data_fill++ = v.x * scale;
      *data_fill++ = v.y * scale;
      *data_fill++ = 1.0f;
      *data_fill++ = color.r;
      *data_fill++ = color.g;
      *data_fill++ = color.b;
      *data_fill++ = 0.5f;

      *data_frame++ = v.x * scale;
      *data_frame++ = v.y * scale;
      *data_frame++ = 1.0f;
      *data_frame++ = color.r;
      *data_frame++ = color.g;
      *data_frame++ = color.b;
      *data_frame++ = 0.5f;

	  	theta += k_increment;
    }

    component_fill->vertexBuffer()->unlock(data_fill - 1);
    component_frame->vertexBuffer()->unlock(data_frame - 1);

    app()->graphics()->renderer()->addForRendering(component_fill);
    app()->graphics()->renderer()->addForRendering(component_frame);
  }

	//b2Vec2 p = center + radius * axis;
	//glBegin(GL_LINES);
	//glVertex2f(center.x, center.y);
	//glVertex2f(p.x, p.y);
	//glEnd();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
  PRenderComponent component = ege_new RenderComponent(app(), "DebugDraw::DrawSegment", EGEGraphics::RP_PHYSICS_DEBUG, 
                                                       EGEGraphics::RPT_LINES);
  if (component->isValid())
  {
    float32 scale = manager()->simulationToWorldScaleFactor();
 
    component->vertexBuffer()->setSemantics(EGEVertexBuffer::ST_V3_C4);

    float32* data = (float32*) component->vertexBuffer()->lock(0, 2);

    *data++ = p1.x * scale;
    *data++ = p1.y * scale;
    *data++ = 1.0f;
    *data++ = color.r;
    *data++ = color.g;
    *data++ = color.b;
    *data++ = 1.0f;

    *data++ = p2.x * scale;
    *data++ = p2.y * scale;
    *data++ = 1.0f;
    *data++ = color.r;
    *data++ = color.g;
    *data++ = color.b;
    *data++ = 1.0f;

    component->vertexBuffer()->unlock(data - 1);

    app()->graphics()->renderer()->addForRendering(component);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void DebugDraw::DrawTransform(const b2Transform& xf)
{
	//b2Vec2 p1 = xf.position, p2;
	//const float32 k_axisScale = 0.4f;
	//glBegin(GL_LINES);
	//
	//glColor3f(1.0f, 0.0f, 0.0f);
	//glVertex2f(p1.x, p1.y);
	//p2 = p1 + k_axisScale * xf.R.col1;
	//glVertex2f(p2.x, p2.y);

	//glColor3f(0.0f, 1.0f, 0.0f);
	//glVertex2f(p1.x, p1.y);
	//p2 = p1 + k_axisScale * xf.R.col2;
	//glVertex2f(p2.x, p2.y);

	//glEnd();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_PHYSICS_BOX2D