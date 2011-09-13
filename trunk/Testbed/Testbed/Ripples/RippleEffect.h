#ifndef RIPPLEEFFECT_H
#define RIPPLEEFFECT_H

#include <EGETexture.h>
#include <EGEScene.h>
#include <EGEGraphics.h>
#include <EGETime.h>
#include <EGEInput.h>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#define RIPPLE_COUNT 3
#define GRID_SIZE_X  32
#define GRID_SIZE_Y  32
#define RIPPLE_LENGTH     2048
#define RIPPLE_CYCLES     18
#define RIPPLE_AMPLITUDE  0.125
#define RIPPLE_STEP	  7

typedef struct {	/* precomputed displacement vector table */
  float dx[2];
  int r;		/* distance from origin, in pixels */
} RIPPLE_VECTOR;

typedef struct {	/* precomputed ripple amplitude table */
  float amplitude;
} RIPPLE_AMP;

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
    bool initialize(EGE::s32 width, EGE::s32 height, EGE::PCamera camera);
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
    /*! Grid locations of ripple centers. */
    EGE::Vector2i m_centers[RIPPLE_COUNT];
    /*! Ripple animation times. */
    EGE::s32 m_times[RIPPLE_COUNT];
    /*! Max ripple animation times. */
    EGE::s32 m_maxTimes[RIPPLE_COUNT];
    /*! Default texture coords for each grid corner. */
    EGE::Vector2f m_defaultTextureCoords[GRID_SIZE_X][GRID_SIZE_Y];
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // RIPPLEEFFECT_H