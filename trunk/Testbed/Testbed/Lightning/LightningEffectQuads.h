#ifndef LIGHTNINGEFFECTQUADS_H
#define LIGHTNINGEFFECTQUADS_H

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

class LightningEffectQuads
{
  public:

    LightningEffectQuads(EGE::Application* app);
   ~LightningEffectQuads();

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
    void render(EGE::IRenderer* renderer);
    /*! Returns render data. */
    inline EGE::PRenderComponent renderData() const { return m_renderData; }
    /* Sets maximum segment midpoint offset. */
    void setMaxSegmentOffset(EGE::float32 offset);
    /* Set offshot angle. */
    void setOffshotAngle(const EGE::Angle& angle);
    /* Set offshot angle variance. */
    void setOffshotAngleVariance(const EGE::Angle& angle);
    /* Sets width. */
    void setWidth(EGE::float32 width);

  private:

    /* Clears object. */
    void clear();
    /* Generates render data based on segments. */
    void generateRenderData();

  private:

    /*! Segment data class. */
    class Segment
    {
      public:

        Segment() : begining(false), ending(NULL), prev(NULL), next(NULL), offshot(NULL) {}

        EGE_DEFINE_NEW_OPERATORS_INLINE
        EGE_DEFINE_DELETE_OPERATORS_INLINE

      public: 

        EGE::Vector2f start;              /*!< Segment start point. */
        EGE::Vector2f end;                /*!< Segment end point. */
        EGE::Vector2f normal;             /*!< Normalized segment normal vector. */
      
        EGE::float32 intensity;           /*!< Segment intensity [0-1]. */
      
        bool begining;                    /*!< TRUE if segment is a begining segment. */
        bool ending;                      /*!< TRUE if segment is an ending segment. */

        Segment* prev;                    /*!< Previous segment this one is connected to. Can be NULL. */
        Segment* next;                    /*!< Next segmnet this one is connected to. Can be NULL. */
        Segment* offshot;                 /*!< Offshot segment this one is connected to. Can be NULL. */
    };

    typedef EGE::List<Segment*> SegmentList;

  private:

    /*! Pointer to application object. */
    EGE::Application* m_app;
    /*! Render data. */
    EGE::PRenderComponent m_renderData;
    /*! List of currently generated segments. */
    SegmentList m_segments;
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

    EGE::Time m_fadeTime;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // LIGHTNINGEFFECTQUADS_H