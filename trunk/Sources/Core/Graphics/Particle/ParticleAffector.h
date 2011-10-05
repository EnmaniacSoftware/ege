#ifndef EGE_CORE_PARTICLEAFFECTOR_H
#define EGE_CORE_PARTICLEAFFECTOR_H

/** 
*   This is base object for specialization of different particle affectors. Particle affector is an object responsible for modification of spawn particles
*   according to predefined data.
*/

#include <EGE.h>
#include <EGETime.h>
#include <EGEString.h>
#include <EGEDictionary.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DECLARE_SMART_CLASS(ParticleAffector, PParticleAffector)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class ParticleAffector : public Object
{
  public:

    ParticleAffector(Application* app, const String& name);
    virtual ~ParticleAffector();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /* Initializes affector from dictionary. */
    virtual bool initialize(const Dictionary& params);
    
  protected:

    /*! Active flag. */
    bool m_active;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_PARTICLEAFFECTOR_H
