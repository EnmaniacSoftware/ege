#ifndef LIGHTNINGEFFECTSTRIPS_H
#define LIGHTNINGEFFECTSTRIPS_H

/** Class implementing the textured-quads lightining effect. 
 *  Effect consists of number of segments where each segments consists of three (3) quads:
 *  - begining
 *  - middle
 *  - ending
 *
 *  Optionally each segment can generate offshot from main lightning beam. Offshots can further subdivide and have offshots of their own.
 */

#include <EGEGraphics.h>
#include <EGETime.h>
#include <EGEInput.h>
#include <EGERandom.h>
#include <EGEList.h>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class LightningEffectStrips
{
  public:

    LightningEffectStrips(EGE::Application* app);
   ~LightningEffectStrips();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! Available states. */
    enum State
    {
      STATE_NONE,           /*!< Uninitialized. */
      STATE_IDLE,
      STATE_BUSY,
    };

  public:

    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /* Generates segments. */
    void create(const EGE::Vector2f& start, const EGE::Vector2f end, EGE::s32 steps, bool allowOffshots);
    /* Starts effect. */
    void start();
    /*! Returns current state. */
    inline State state() const { return m_state; }
    /* Updates effect. */
    void update(const EGE::Time& time);
    /* Renders object. */
    void render(EGE::Renderer* renderer);
    /* Sets maximum segment midpoint offset. */
    void setMaxSegmentOffset(EGE::float32 offset);
    /* Set offshot angle. */
    void setOffshotAngle(const EGE::Angle& angle);
    /* Set offshot angle variance. */
    void setOffshotAngleVariance(const EGE::Angle& angle);
    /* Sets width. */
    void setWidth(EGE::float32 width);
    /* Sets material. */
    void setMaterial(const EGE::PMaterial& material);
    /* Sets render priority. */
    void setRenderPriority(EGE::s32 priority);

  private:

    /* Clears object. */
    void clear();
    /* Generates render data based on segments. */
    void generateRenderData();

  private:

    /*! Segment data struct. */
    struct Segment
    {
      EGE::Vector2f start;
      EGE::Vector2f end;
      EGE::Vector2f normal;
      EGE::float32 intensity;
    };

    struct Beam
    {
      EGE::List<Segment> segments;
      EGE::PRenderComponent renderData;
    };

    typedef EGE::List<Segment> SegmentList;
    typedef EGE::List<Beam> BeamList;

  private:

    /*! Pointer to application object. */
    EGE::Application* m_app;
    /*! Randomizer. */
    EGE::Random m_random;
    /*! List of currently generated beams. */
    BeamList m_beams;
    /*! Maximum segment midpoint offset. */
    EGE::float32 m_maximumOffset;
    /*! Offshot angle. */
    EGE::Angle m_offshotAngle;
    /*! Offshot angle variance. */
    EGE::Angle m_offshotAngleVariance;
    /*! Current state. */
    State m_state;
    /*! Width. */
    EGE::float32 m_width;
    /*! Material. */
    EGE::PMaterial m_material;
    /*! Render priority. */
    EGE::s32 m_renderPriority;
    EGE::Time m_fadeTime;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // LIGHTNINGEFFECTSTRIPS_H