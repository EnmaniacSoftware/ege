#ifndef TEST_CURVES_H
#define TEST_CURVES_H

#include "Test.h"
#include "RenderObject.h"
#include <EGEGraphics.h>
#include <EGESpline.h>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class CurvesTest : public Test
{
  public:

    CurvesTest(App* app);
    virtual ~CurvesTest();

    /* Test override. Returns test name. */
    EGE::String name() const override;
    /* Test override. Initializes test. */
    bool initialize() override;
    /* Test override. Updates test. */
    void update(const EGE::Time& time) override;

  private slots:

    /* Slot called before target is rendered. */
    void preRender(EGE::PRenderTarget target);

  private:

    /*! Scene object. */
    RenderObject* m_sceneObject;
    /*! Spline objects. */
    EGE::CubicSpline m_splines[4];
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // TEST_CURVES_H