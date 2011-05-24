#include "Core/Application/Application.h"
#include "Core/Physics/Box2D/DebugDrawBox2D.h"
#include "Core/Physics/Box2D/PhysicsManagerBox2D_p.h"
#include "EGEGraphics.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(DebugDraw)
EGE_DEFINE_DELETE_OPERATORS(DebugDraw)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DebugDraw::DebugDraw(Application* app, PhysicsManagerPrivate* managerPrivate) : m_app(app), m_managerPrivate(managerPrivate)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DebugDraw::~DebugDraw()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! b2DebugDraw override. */
void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
  //RenderComponent* component = ege_new RenderComponent(app(), "DebugDraw::DrawPolygon");
  //if (component->isValid())
  //{
  //  component->vertexBuffer()->addBuffer(VertexBuffer::ARRAY_TYPE_POSITION_XYZ);
  //  component->vertexBuffer()->addBuffer(VertexBuffer::ARRAY_TYPE_COLOR_RGBA);

  //  component->vertexBuffer()->lock(0, 
  //}

	//glColor3f(color.r, color.g, color.b);
	//glBegin(GL_LINE_LOOP);
	//for (int32 i = 0; i < vertexCount; ++i)
	//{
	//	glVertex2f(vertices[i].x, vertices[i].y);
	//}
	//glEnd();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! b2DebugDraw override. */
void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	//glEnable(GL_BLEND);
	//glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glColor4f(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);
	//glBegin(GL_TRIANGLE_FAN);
	//for (int32 i = 0; i < vertexCount; ++i)
	//{
	//	glVertex2f(vertices[i].x, vertices[i].y);
	//}
	//glEnd();
	//glDisable(GL_BLEND);

	//glColor4f(color.r, color.g, color.b, 1.0f);
	//glBegin(GL_LINE_LOOP);
	//for (int32 i = 0; i < vertexCount; ++i)
	//{
	//	glVertex2f(vertices[i].x, vertices[i].y);
	//}
	//glEnd();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! b2DebugDraw override. */
void DebugDraw::DrawCircle(const b2Vec2& center, ::float32 radius, const b2Color& color)
{
	//const float32 k_segments = 16.0f;
	//const float32 k_increment = 2.0f * b2_pi / k_segments;
	//float32 theta = 0.0f;
	//glColor3f(color.r, color.g, color.b);
	//glBegin(GL_LINE_LOOP);
	//for (int32 i = 0; i < k_segments; ++i)
	//{
	//	b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
	//	glVertex2f(v.x, v.y);
	//	theta += k_increment;
	//}
	//glEnd();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! b2DebugDraw override. */
void DebugDraw::DrawSolidCircle(const b2Vec2& center, ::float32 radius, const b2Vec2& axis, const b2Color& color)
{
	//const float32 k_segments = 16.0f;
	//const float32 k_increment = 2.0f * b2_pi / k_segments;
	//float32 theta = 0.0f;
	//glEnable(GL_BLEND);
	//glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glColor4f(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);
	//glBegin(GL_TRIANGLE_FAN);
	//for (int32 i = 0; i < k_segments; ++i)
	//{
	//	b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
	//	glVertex2f(v.x, v.y);
	//	theta += k_increment;
	//}
	//glEnd();
	//glDisable(GL_BLEND);

	//theta = 0.0f;
	//glColor4f(color.r, color.g, color.b, 1.0f);
	//glBegin(GL_LINE_LOOP);
	//for (int32 i = 0; i < k_segments; ++i)
	//{
	//	b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
	//	glVertex2f(v.x, v.y);
	//	theta += k_increment;
	//}
	//glEnd();

	//b2Vec2 p = center + radius * axis;
	//glBegin(GL_LINES);
	//glVertex2f(center.x, center.y);
	//glVertex2f(p.x, p.y);
	//glEnd();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! b2DebugDraw override. */
void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
  PRenderComponent component = ege_new RenderComponent(app(), "DebugDraw::DrawSegment");
  if (component->isValid())
  {
    EGE::float32 scale = manager()->simulationToWorldScaleFactor();
 
    component->setPrimitiveType(RenderComponent::PRIMITIVE_TYPE_LINES);
    component->vertexBuffer()->addBuffer(VertexBuffer::ARRAY_TYPE_POSITION_XYZ);
    component->vertexBuffer()->addBuffer(VertexBuffer::ARRAY_TYPE_COLOR_RGBA);

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

    component->vertexBuffer()->unlock();

    app()->graphics()->renderer()->addForRendering(TMatrix4f::IDENTITY, component);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! b2DebugDraw override. */
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