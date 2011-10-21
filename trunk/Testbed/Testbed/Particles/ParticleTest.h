#ifndef TEST_PARTICLE_H
#define TEST_PARTICLE_H

#include "Test.h"
#include "RenderObject.h"
#include <EGEParticle.h>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class ParticleTest : public Test
{
  public:

    ParticleTest(App* app);
    virtual ~ParticleTest();

    /* Test override. Returns test name. */
    EGE::String name() const override;
    /* Test override. Initializes test. */
    bool initialize() override;
    /* Test override. Updates test. */
    void update(const EGE::Time& time) override;

  private:

    /*! Particle emitter. */
    EGE::PParticleEmitter m_emitter[2];
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // TEST_PARTICLE_H