#ifndef RIPPLEEFFECT_H
#define RIPPLEEFFECT_H

#include <EGETexture.h>
#include <EGEScene.h>
#include <EGEGraphics.h>
#include <EGETime.h>
#include <EGEInput.h>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#define RIPPLE_COUNT 1
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

  private:

    /* SceneNodeObject override. Adds object render data for rendering with given renderer. */
    bool addForRendering(EGE::Renderer* renderer) override;

  private:

    /*! Pointer to application object. */
    EGE::Application* m_app;
    /*! Render data. */
    EGE::PRenderComponent m_renderData;
    /*! Render texture. */
    EGE::PTexture2D m_texture;
    /*! Current state. */
    State m_state;

    EGE::s32 cx[RIPPLE_COUNT];						// grid X location of ripple center
    EGE::s32 cy[RIPPLE_COUNT];						// grid Y location of ripple center
    EGE::s32 t[RIPPLE_COUNT];							// anim time for ripplets
    EGE::s32 max[RIPPLE_COUNT];						// max anim time (if time is less that this, anim will be considered still working)
    EGE::float32 dt[GRID_SIZE_X][GRID_SIZE_Y][2];

    EGE::s32 m_maxRipple;

};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // RIPPLEEFFECT_H