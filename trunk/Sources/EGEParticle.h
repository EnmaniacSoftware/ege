#ifndef EGE_PARTICLE_H
#define EGE_PARTICLE_H

#include "Core/Platform.h"
#include <EGEVector.h>
#include <EGEColor.h>
#include <EGETime.h>
#include <EGEDynamicArray.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

namespace EGEParticle
{
  /*! Particle data structure. */
  struct ParticleData
  {
    Vector3f position;            /*!< Current position. */ 
    Vector3f velocity;            /*!< Velocity vector. */
    Vector3f acceleration;        /*!< Acceleration vector. */

    Color color;                  /*!< Current color. */
    Color colorDelta;             /*!< Color delta. */

    Vector2f size;                /*!< Current size. */
    Vector2f sizeDelta;           /*!< Size delta. */

    Time timeLeft;                /*!< Time left to die. */
  };
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

typedef DynamicArray<EGEParticle::ParticleData> ParticleDataArray;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "Core/Graphics/Particle/ParticleEmitter.h"
#include "Core/Graphics/Particle/ParticleAffector.h"
#include "Core/Graphics/Particle/ParticleFactory.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EGE_PARTICLE_H