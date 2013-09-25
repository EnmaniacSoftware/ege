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

EGE_DECLARE_SMART_CLASS(LightningEffectStrips, PLightningEffect)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class LightningEffectStrips : public EGE::Object
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
    void create(const EGE::List<EGE::Vector2f>& points, EGE::s32 steps, bool allowOffshots);
    /* Starts effect. */
    void start();
    /*! Returns current state. */
    inline State state() const { return m_state; }
    /* Updates effect. */
    void update(const EGE::Time& time);
    /* Renders object. */
    void render(EGE::IRenderer* renderer);
    /* Sets maximum segment midpoint offset. */
    void setMaxSegmentOffset(EGE::float32 offset);
    /* Sets offshot angle. */
    void setOffshotAngle(const EGE::Angle& angle);
    /* Sets offshot angle variance. */
    void setOffshotAngleVariance(const EGE::Angle& angle);
    /* Sets randmization variance. */
    void setRandmizationVariance(EGE::float32 variance);
    /* Sets randomization interval. */
    void setRandomizationInterval(const EGE::Time& time);
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
      EGE::Vector2f start;                          /*!< Start point. */
      EGE::Vector2f end;                            /*!< End point. */
      EGE::Vector2f normal;                         /*!< Normal (from start and end points). */
      EGE::Vector2f randomizationNormal;            /*!< Randomization normal (this may be different from normal). */
      EGE::float32 randomization;                   /*!< Current randomization value. */
      EGE::float32 intensity;                       /*!< Intensity. */
    };

    /*! Beam data struct. */
    struct Beam
    {
      EGE::List<Segment> segments;                  /*!< List of all segments. */
      EGE::PRenderComponent renderData;             /*!< Render data. */
    };

    typedef EGE::List<Segment> SegmentList;
    typedef EGE::List<Beam> BeamList;

  private:

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
    /*! Next randomization time. */
    EGE::Time m_randomizationTime;
    /*! Randomization variance. */
    EGE::float32 m_randomizationVariance;
    /*! Randomization interval. */
    EGE::Time m_randomizationInterval;

    EGE::Time m_fadeTime;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // LIGHTNINGEFFECTSTRIPS_H