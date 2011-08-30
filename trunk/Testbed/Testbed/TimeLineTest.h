#ifndef TEST_TIMELINE_H
#define TEST_TIMELINE_H

#include "Test.h"
#include "RenderObject.h"
#include <EGETimeline.h>
#include <EGEGraphics.h>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class TimeLineTest : public Test
{
  public:

    TimeLineTest(App* app);
    virtual ~TimeLineTest();

    /* Test override. Returns test name. */
    EGE::String name() const override;
    /* Test override. Creates scene. */
    bool createScene() override;
    /* Test override. Updates test. */
    void update(const EGE::Time& time) override;

  private slots:

    /* Slot called when time line frame has been changed. */
    void frameChanged(EGE::s32 frame);
    /* Slot called when time line finishes. */
    void finished();

  private:

    /*! Time line. */
    EGE::PTimeLine m_timeLine;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // TEST_TIMELINE_H