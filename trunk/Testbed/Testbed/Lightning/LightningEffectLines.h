#ifndef LIGHTNINGEFFECTLINES_H
#define LIGHTNINGEFFECTLINES_H

#include <EGEScene.h>
#include <EGEGraphics.h>
#include <EGETime.h>
#include <EGEInput.h>
#include <EGERandom.h>
#include <EGEList.h>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class LightningEffectLines : public EGE::SceneNodeObject
{
  public:

    LightningEffectLines(EGE::Application* app);
    virtual ~LightningEffectLines();

  public:

    /* Creates lightning effect. */
    void create(const EGE::Vector2f& start, const EGE::Vector2f end, EGE::s32 steps, bool allowOffshots);
    /* Updates effect. */
    void update(const EGE::Time& time);
    /* Pointer event receiver. */
    void pointerEvent(EGE::PPointerData data);

  private:

    /* SceneNodeObject override. Adds object render data for rendering with given renderer. */
    bool addForRendering(EGE::Renderer* renderer, const EGE::Matrix4f& transform = EGE::Matrix4f::IDENTITY) override;

  private:

    /*! Segment data struct. */
    struct Segment
    {
      EGE::Vector2f start;
      EGE::Vector2f end;
      EGE::Vector2f normal;
      EGE::float32 intensity;
    };

    typedef EGE::List<Segment> SegmentList;

  private:

    /*! Pointer to application object. */
    EGE::Application* m_app;
    /*! Render data. */
    EGE::PRenderComponent m_renderData;
    /*! Randomizer. */
    EGE::Random m_random;
    /*! Currently generated segments. */
    SegmentList m_segments;
    EGE::Time m_fadeTime;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // LIGHTNINGEFFECTLINES_H