#ifndef EGE_CORE_PHYSICS_DEBUG_DRAW_BOX2D_H
#define EGE_CORE_PHYSICS_DEBUG_DRAW_BOX2D_H

#ifdef EGE_PHYSICS_BOX2D

#include <EGE.h>
#include <Box2D/Box2D.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class PhysicsManagerPrivate;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class DebugDraw : public b2DebugDraw
{
  public:

    DebugDraw(Engine& engine, PhysicsManagerPrivate* managerPrivate);
   ~DebugDraw();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /*! @see b2DebugDraw::DrawPolygon. */
	  void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;
    /*! @see b2DebugDraw::DrawSolidPolygon. */
	  void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;
    /*! @see b2DebugDraw::DrawCircle. */
	  void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) override;
    /*! @see b2DebugDraw::DrawSolidCircle. */
	  void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) override;
    /*! @see b2DebugDraw::DrawSegment. */
	  void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override;
    /*! @see b2DebugDraw::DrawTransform. */
	  void DrawTransform(const b2Transform& xf) override;

  private:

    /*! Returns engine object. */
    Engine& engine() const;
    /*! Returns pointer to Box2D physics manager. */
    PhysicsManagerPrivate* manager() const { return m_managerPrivate; }

  private:

    /*! Reference to engine. */
    Engine& m_engine;
    /*! Pointer to Box2D physics manager. */
    PhysicsManagerPrivate* m_managerPrivate;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_PHYSICS_BOX2D

#endif // EGE_CORE_PHYSICS_DEBUG_DRAW_BOX2D_H
