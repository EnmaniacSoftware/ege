#ifndef RIPPLEEFFECT_H
#define RIPPLEEFFECT_H

#include <EGETexture.h>
#include <EGEScene.h>
#include <EGEGraphics.h>
#include <EGETime.h>
#include <EGEInput.h>
#include <EGEDynamicArray.h>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#define RIPPLE_COUNT 3

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class RippleEffect : public EGE::SceneNodeObject
{
  public:

    RippleEffect(EGE::Application* app);
    virtual ~RippleEffect();

  public:

    /*! Available states. */
    enum State
    {
      STATE_IDLE,
      STATE_BUSY
    };

    /* Initializes object. */
    bool initialize(EGE::s32 width, EGE::s32 height, const EGE::Vector2i& gridSize, EGE::PCamera camera);
    /*! Returns render texture. */
    EGE::PTexture2D texture() const { return m_texture; }
    /* Updates effect. */
    void update(const EGE::Time& time);
    /* Pointer event receiver. */
    void pointerEvent(EGE::PPointerData data);
    /* Sets speed. */
    void setSpeed(EGE::s32 speed);

  private:

    /* SceneNodeObject override. Adds object render data for rendering with given renderer. */
    bool addForRendering(EGE::Renderer* renderer) override;
    /* Computes the distance of the given window coordinate to the nearest window corner (in pixels). */
    EGE::s32 rippleMaxDistance(const EGE::Vector2i& pos) const;
    /* Precalculates data. */
    void precalculate();

  private:

    /*! Precomputed displacement vector table entry. */
    struct RIPPLEENTRY
    {
      EGE::float32 dx[2];
      EGE::s32 r;		          /*!< Distance from origin (in pixels). */
    };

  private:

    /*! Pointer to application object. */
    EGE::Application* m_app;
    /*! Render data. */
    EGE::PRenderComponent m_renderData;
    /*! Render texture. */
    EGE::PTexture2D m_texture;
    /*! Current state. */
    State m_state;
    /*! Ripple speed. */
    EGE::s32 m_speed;
    /*! Effect area width (in pixels). */
    EGE::s32 m_width;
    /*! Effect area height (in pixels). */
    EGE::s32 m_height;
    /*! Grid size. */
    EGE::Vector2i m_gridSize;
    /*! Grid locations of ripple centers. */
    EGE::Vector2i m_centers[RIPPLE_COUNT];
    /*! Ripple animation times. */
    EGE::s32 m_times[RIPPLE_COUNT];
    /*! Max ripple animation times. */
    EGE::s32 m_maxTimes[RIPPLE_COUNT];
    /*! Default texture coords for each grid corner. */
    EGE::DynamicArray<EGE::Vector2f> m_defaultTextureCoords;
    /*! Array of precomputed displacement values. */
    EGE::DynamicArray<RIPPLEENTRY> m_vector;
    /*! Array of precomputed amplitudes values. */
    EGE::DynamicArray<EGE::float32> m_amplitudes;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // RIPPLEEFFECT_H