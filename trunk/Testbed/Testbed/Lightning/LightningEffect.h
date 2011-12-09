#ifndef LIGHTNINGEFFECT_H
#define LIGHTNINGEFFECT_H

#include <EGETexture.h>
#include <EGEScene.h>
#include <EGEGraphics.h>
#include <EGETime.h>
#include <EGEInput.h>
#include <EGEDynamicArray.h>
#include <EGERandom.h>

struct Segment
{
  EGE::Vector2f start;
  EGE::Vector2f end;
  EGE::Vector2f normal;       // normalized
  EGE::Vector2f startNormal;       // normalized
  EGE::Vector2f endNormal;         // normalized
  EGE::float32 intensity;
  bool _begin;
  bool _end;
};

struct Segment2
{
  EGE::Vector2f start;
  EGE::Vector2f end;
  EGE::Vector2f normal;             // normalized
  EGE::float32 intensity;
  bool _begin;
  bool _end;

  Segment2* prev;
  Segment2* next;
  Segment2* offshot;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class LightningEffect : public EGE::SceneNodeObject
{
  public:

    LightningEffect(EGE::Application* app);
    virtual ~LightningEffect();

  public:

    /* Updates effect. */
    void update(const EGE::Time& time);
    /* Pointer event receiver. */
    void pointerEvent(EGE::PPointerData data);

  private:

    /* SceneNodeObject override. Adds object render data for rendering with given renderer. */
    bool addForRendering(EGE::Renderer* renderer) override;
    /* Generates segments. */
    EGE::List<Segment> generateSegments(const EGE::Vector2f& start, const EGE::Vector2f end, EGE::s32 steps, bool allowOffshots);
    /* Generates segments. */
    EGE::List<Segment2*> generateSegments2(const EGE::Vector2f& start, const EGE::Vector2f end, EGE::s32 steps, bool allowOffshots);

  private:

    /*! Pointer to application object. */
    EGE::Application* m_app;
    /*! Render data. */
    EGE::PRenderComponent m_renderData;
    /*! Randomizer. */
    EGE::Random m_random;
    /*! Render data. */
    EGE::PRenderComponent m_renderDataQuad;
    /*! Render data. */
    EGE::PRenderComponent m_renderDataQuad2;

    EGE::List<Segment> m_segments;
    EGE::Time m_fadeTime;

    EGE::List<Segment2*> m_segments2;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // LIGHTNINGEFFECT_H