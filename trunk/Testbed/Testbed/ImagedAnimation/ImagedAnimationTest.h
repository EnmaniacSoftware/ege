#ifndef TEST_IMAGED_ANIMATION_H
#define TEST_IMAGED_ANIMATION_H

#include "Test.h"
#include <EGEImagedAnimation.h>
#include <EGEGraphics.h>
#include <EGEList.h>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class ImagedAnimationTest : public Test
{
  public:

    ImagedAnimationTest(App* app);
    virtual ~ImagedAnimationTest();

    /* Test override. Returns test name. */
    EGE::String name() const override;
    /* Test override. Initializes test. */
    bool initialize() override;
    /* Test override. Updates test. */
    void update(const EGE::Time& time) override;

  private:

    /* Test override. Pointer event receiver. */
    void pointerEvent(EGE::PPointerData data) override;

  private slots:

    /* Slot called before target is rendered. */
    void preRender(EGE::PRenderTarget target);
    /* Test override. Slot called when resource group has been loaded. */
    void groupLoadComplete(const EGE::String& name) override;

  private:

    EGE::List<EGE::PImagedAnimation> m_anims;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // TEST_IMAGED_ANIMATION_H